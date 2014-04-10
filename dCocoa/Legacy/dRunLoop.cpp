/*
** $Id: 2009/05/13, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dRunLoop.h"

#include "dThread.h"
#include "dTimer.h"
#include "dValue.h"



CL_CLASS_DEF(dRunLoop, dCocoa)


dRunLoop::dRunLoop()
: _ownThread(nil),
  _activeTimers(nil) {
#if defined (IS_WIN)
    _oldHook = nil;
#endif
}

dRunLoop*
dRunLoop::initWithOwnThread(dThread* owner) {
    if (!_super::init()) return nil;

	CL_THROW_IF_NIL(owner);
    _ownThread = owner;

#if defined (IS_WIN)
    _setHook();
#endif

    return this;
}

void
dRunLoop::dealloc() {
    if (_activeTimers) {
        dSetT<dTimer*>::Iterator it(_activeTimers);
        while (it.next()) _uninstallTimer(it.object());
        _activeTimers->release();
    }

#if defined (IS_WIN)
    _removeHook();
#endif

    _super::dealloc();
}

dRunLoop*
dRunLoop::mainThreadRunLoop() {
    return dThread::mainThread()->runLoop();
}

void
dRunLoop::_installTimer(dTimer* timer) {
	CL_THROW_IF_NIL(timer);
    if (!_activeTimers)
        _activeTimers = CL_NEW(dSetT<dTimer*>);

#if defined (IS_WIN)
    UINT_PTR timerID = ::SetTimer(
                            nil,
                            0,
                            timer->interval(),
                            nil);
#elif defined (IS_MAC)
    CFRunLoopTimerContext context = { 0, this, nil, nil };
    CFTimeInterval ti = timer->interval() * 0.001;
    CFRunLoopTimerRef timerID = CFRunLoopTimerCreate(
                                    nil,
                                    CFAbsoluteTimeGetCurrent() + ti,
                                    timer->_repeats ? ti : 0,
                                    0,
                                    0,
                                    _timerCB,
                                    &context);
    
    CFRunLoopAddTimer(CFRunLoopGetCurrent(), timerID, kCFRunLoopCommonModes);
#endif

    dobj_ptr<dValue>    timerValueID(dValue::alloc()->initWithInt((dInteger)timerID));
    timer->_setTimerID(timerValueID.get());
    _activeTimers->addObject(timer);
}

void
dRunLoop::_uninstallTimer(dTimer* timer) {
    if (!timer->_timerID || !_activeTimers || !_activeTimers->containsObject(timer))
        throw new ExNotInstalledTimer;

#if defined (IS_WIN)
    ::KillTimer(nil, timer->_timerID->intValue());
#elif defined (IS_MAC)
    CFRunLoopTimerRef timerID = (CFRunLoopTimerRef)timer->_timerID->intValue();
    if (CFRunLoopTimerIsValid(timerID))
        CFRunLoopTimerInvalidate(timerID);
    CFRelease(timerID);
#endif

    _activeTimers->removeObject(timer);
}

bool
dRunLoop::_processTimer(dValue* timerID) {
    if (!_activeTimers) return false;

    dSetT<dTimer*>::Iterator it(_activeTimers);
    while (it.next()) {
        if (it.object()->_timerID->isEqual(timerID)) {
            it.object()->fire();
            return true;
        }
    }   
    return false;
}

#if defined (IS_WIN)
bool
dRunLoop::_processMessage(DWORD msg, WPARAM wParam, LPARAM lParam) {
    _LocalPool

    switch (msg) {
        case WM_TIMER: {
            if (0 != lParam) break;
            dobj_ptr<dValue>    timerID(dValue::alloc()->initWithInt(wParam));
            return _processTimer(timerID.get());
        }
    }
    return false;
}

void
dRunLoop::_setHook() {
    if (_oldHook) return;

    _oldHook = ::SetWindowsHookEx(
        WH_GETMESSAGE,
        &dRunLoop::_hookProc,
        nil,
        _ownThread->_threadID);
}

void
dRunLoop::_removeHook() {
    ::UnhookWindowsHookEx(_oldHook);
    _oldHook = nil;
}

LRESULT CALLBACK
dRunLoop::_hookProc(int code, WPARAM wParam, LPARAM lParam) {
    dRunLoop* rl = dThread::currentThread()->runLoop();
    if (HC_ACTION == code && PM_REMOVE == (PM_REMOVE & wParam)) {
        LPMSG msg = (LPMSG)lParam;
        if (rl->_processMessage(msg->message, msg->wParam, msg->lParam)) {
            msg->message = WM_NULL;
            msg->wParam = 0;
            msg->lParam = 0;
            return 0;
        }
    }

    return ::CallNextHookEx(rl->_oldHook, code, wParam, lParam);
}
#elif defined (IS_MAC)
void
dRunLoop::_timerCB(CFRunLoopTimerRef timerID, void* info) {
    dobj_ptr<dValue>    timerVaslueID(dValue::alloc()->initWithInt((dInteger)timerID));
    ((dRunLoop*)info)->_processTimer(timerVaslueID.get());
}
#endif
