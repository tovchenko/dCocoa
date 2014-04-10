/*
** $Id: 2009/03/23, Taras Tovchenko $
** Thread and other utilities functionality for it - synchronization, tls
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DTHREAD_H
#define _COCOA_DTHREAD_H

#if defined (IS_WIN)
    #include <windows.h>
#endif

#define _LockScope(criticalSection)     dCriticalSection::StLocker((criticalSection), true);



BEGIN_NAMESPACE_CL
class dRunLoop;

class DLL_PUBLIC dThread : public dObject {
friend class dRunLoop;          // use _threadID;

CL_CLASS_DECL(dThread, dObject)
public:
    static dThread*             mainThread();
    bool                        isMainThread() const;

    static dThread*             currentThread();
    // run loop
    dRunLoop*                   runLoop();

protected:
    dThread();
    virtual void                dealloc();

    static void                 _staticCtor();
    static void                 _staticDtor();

    dRunLoop*                   _runLoop;
#if defined (IS_WIN)
    HANDLE                      _threadHandle;
    DWORD                       _threadID;
#elif defined (IS_MAC)
    pthread_t                   _threadID;
#endif
};

class DLL_PUBLIC dTLS : public dObject {
CL_CLASS_DECL(dTLS, dObject)
public:
    // exceptions
    class ExGetValueFalse : public CL::CException { };
    // initializers
    virtual dTLS*               init();
    // get / set pointer on your value to storage
    void*                       value() const;
    void                        setValue(void* val);

protected:
    dTLS();
    virtual void                dealloc();

#if defined (IS_WIN)
    DWORD                       _index;
#elif defined (IS_MAC)
    pthread_key_t               _index;
#endif
};

class DLL_PUBLIC dCriticalSection : public dObject {
CL_CLASS_DECL(dCriticalSection, dObject)
public:
    class DLL_PUBLIC StLocker {
    public:
        StLocker(dCriticalSection* resource, bool needWait = true);
        ~StLocker();

        bool                    wasLocked() const;

    protected:
        dCriticalSection*       _resource;
        bool                    _wasLocked;
    };

    virtual dCriticalSection*   init();
    // controlling of sharable resource
    void                        lock();
    void                        unlock();
    bool                        trylock();

protected:
    dCriticalSection();
    virtual void                dealloc();

#if defined (IS_WIN)
    CRITICAL_SECTION            _criticalSection;
#elif defined (IS_MAC)
    dUInteger                   _lockCount;
    pthread_mutex_t             _mutex;
    pthread_t                   _ownThread;
#endif
};

END_NAMESPACE_CL

#endif // _COCOA_DTHREAD_H