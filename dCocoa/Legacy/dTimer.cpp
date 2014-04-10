/*
** $Id: 2009/05/23, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dTimer.h"

#include "dInvocation.h"
#include "dDict.h"
#include "dThread.h"
#include "dRunLoop.h"
#include "dValue.h"


CL_CLASS_DEF(dTimer, dCocoa)


dTimer::dTimer()
: _interval(0),
  _invocation(nil),
  _userInfo(nil),
  _repeats(false),
  _ownRunLoop(nil),
  _timerID(nil) {
}

dTimer*
dTimer::initWithParams(
           dUInteger interval,
           dInvocation* invocation,
           dDict*  userInfo,
           bool repeats) {

    if (!_super::init()) return nil;

	CL_THROW_IF_NIL(invocation);

    _interval = interval;
    _invocation = invocation->retain();
    _userInfo = userInfo ? userInfo->retain() : nil;
    _repeats = repeats;

    return this;
}

void
dTimer::dealloc() {
    CL_RELEASE(_timerID);
    CL_RELEASE(_invocation);
    CL_RELEASE(_userInfo);

    _super::dealloc();
}

dTimer*
dTimer::scheduledTimerWithTimeInterval(
            dUInteger interval,
            dInvocation* invocation,
            bool repeats) {

    dobj_ptr<dTimer> newTimer(alloc()->initWithParams(
                                    interval,
                                    invocation,
                                    nil,
                                    repeats));

    dRunLoop* rl =  dThread::currentThread()->runLoop();
    newTimer->_ownRunLoop = rl;
    rl->_installTimer(newTimer.get());

    return newTimer.get();
}

dTimer*
dTimer::scheduledTimerWithTimeInterval(
            dUInteger interval,
            dObject* target,
            dSEL sel,
            dDict* userInfo,
            bool repeats)
{
	CL_THROW_IF_TRUE(!target || !sel);
    dobj_ptr<dInvocation>   inv(dInvocation::alloc()->initWithParams(
                                                    target,
                                                    sel,
                                                    userInfo));

    return scheduledTimerWithTimeInterval(interval, inv.get(), repeats);
}

dUInteger
dTimer::interval() const {
    return _interval;
}

void
dTimer::invalidate() {
    if (_timerID)
        _ownRunLoop->_uninstallTimer(this);
}

void
dTimer::fire() const {
    if (_invocation)
        _invocation->invoke();
}

void
dTimer::_setTimerID(dValue* timerID) {
	CL_THROW_IF_NIL(timerID);
    CL_SWITCH_LINKS(_timerID, timerID);
}