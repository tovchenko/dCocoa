/*
** $Id: 2009/05/13, Taras Tovchenko $
** Set hooks for system event handling
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DRUNLOOP_H
#define _COCOA_DRUNLOOP_H

#include "dSet.h"

BEGIN_NAMESPACE_CL
class dThread;
class dTimer;
class dValue;


class DLL_PUBLIC dRunLoop : public dObject {
friend class dThread; //  initWithOwnThread
friend class dTimer;  // _installTimer

CL_CLASS_DECL(dRunLoop, dObject)
public:
    // exceptions
    class ExNotInstalledTimer : public CL::CException { };

    static dRunLoop*            mainThreadRunLoop();

protected:
    virtual dRunLoop*           initWithOwnThread(dThread* owner);

    void                        _installTimer(dTimer* timer);
    void                        _uninstallTimer(dTimer* timer);

    dRunLoop();
    virtual void                dealloc();

    bool                        _processTimer(dValue* timerID);

#if defined (IS_WIN)
    bool                        _processMessage(DWORD msg, WPARAM wParam, LPARAM lParam);

    void                        _setHook();
    void                        _removeHook();
    static LRESULT CALLBACK     _hookProc(int code, WPARAM wParam, LPARAM lParam);

    HHOOK                       _oldHook;
#elif defined (IS_MAC)
    static void                 _timerCB(CFRunLoopTimerRef timerID, void* info);
#endif

    dThread*                    _ownThread;
    dSetT<dTimer*>*             _activeTimers;
};

END_NAMESPACE_CL

#endif // _COCOA_DRUNLOOP_H