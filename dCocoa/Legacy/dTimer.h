/*
** $Id: 2009/05/23, Taras Tovchenko $
** Timer functionality
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DTIMER_H
#define _COCOA_DTIMER_H



BEGIN_NAMESPACE_CL
class dDict;
class dValue;
class dInvocation;
class dRunLoop;

class DLL_PUBLIC dTimer : public dObject {
friend class dRunLoop;  // _setTimerID

CL_CLASS_DECL(dTimer, dObject);
public:
    static dTimer*              scheduledTimerWithTimeInterval(
                                    dUInteger interval,
                                    dInvocation* invocation,
                                    bool repeats);

    static dTimer*              scheduledTimerWithTimeInterval(
                                    dUInteger interval,
                                    dObject* target,
                                    dSEL sel,
                                    dDict* userInfo,
                                    bool repeats);

    dUInteger                   interval() const;
    // stop & release timer
    void                        invalidate();
    void                        fire() const;

protected:
    virtual dTimer*             initWithParams(
                                    dUInteger interval, // in milliseconds
                                    dInvocation* invocation,
                                    dDict*  userInfo,
                                    bool repeats);
    dTimer();
    virtual void                dealloc();

    void                        _setTimerID(dValue* timerID);

    dUInteger                   _interval;
    dInvocation*                _invocation;
    dDict*                      _userInfo;
    bool                        _repeats;

private:
    dRunLoop*                   _ownRunLoop;
    dValue*                     _timerID;
};

END_NAMESPACE_CL

#endif // _COCOA_DTIMER_H