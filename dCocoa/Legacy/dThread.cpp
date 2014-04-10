/*
** $Id: 2009/03/23, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dThread.h"

#include "dDict.h"
#include "dValue.h"
#include "dRunLoop.h"


static dDictT<dThread*, dValue*>*   _threads = nil;

CL_CLASS_DEF_WITH_STATIC_CTORDTOR(dThread,
                                  dCocoa,
                                  &dThread::_staticCtor,
                                  &dThread::_staticDtor)

void
dThread::_staticCtor() {
    // call of mainThread() is necessarily
    mainThread()->runLoop();
}

void
dThread::_staticDtor() {
    CL_RELEASE(_threads);
}

dThread::dThread()
: _runLoop(nil),
  _threadID(0) {
#if defined (IS_WIN)
    _threadHandle = 0;
#endif
}

void
dThread::dealloc() {
    CL_RELEASE(_runLoop);

#if defined (IS_WIN)
    ::CloseHandle(_threadHandle);
#endif
    _super::dealloc();
}

dThread*
dThread::mainThread() {
    static dThread* mainThr = nil;
    if (mainThr) return mainThr;

    mainThr = dThread::currentThread();

    return mainThr;
}

bool
dThread::isMainThread() const {
    return this == mainThread();
}

dThread*
dThread::currentThread() {
    if (!_threads)
        _threads = (dDictT<dThread*, dValue*>*)
            dDictT<dThread*, dValue*>::alloc()->init();

#if defined (IS_WIN)
    DWORD thrID = ::GetCurrentThreadId();
#elif defined (IS_MAC)
    pthread_t thrID = ::pthread_self();
#endif

    dobj_ptr<dValue>    thrKey(dValue::alloc()->initWithInt((dInteger)thrID));
    dThread* result = _threads->objectForKeyNoException(thrKey.get());
    if (result) return result;

    dobj_ptr<dThread>   thrObj(CL_NEW(dThread));

#if defined (IS_WIN)
    ::DuplicateHandle(
        ::GetCurrentProcess(),
        ::GetCurrentThread(),
        ::GetCurrentProcess(),
        &thrObj->_threadHandle,
        0,
        false, 
        DUPLICATE_SAME_ACCESS);
#endif
    
     thrObj->_threadID = thrID;
    _threads->setObjectForKey(thrObj.get(), thrKey.get());
    return thrObj.get();
}

dRunLoop*
dThread::runLoop() {
    if (!_runLoop)
        _runLoop = dRunLoop::alloc()->initWithOwnThread(this);

    return _runLoop;
}


CL_CLASS_DEF(dTLS, dCocoa);

dTLS::dTLS()
: _index(0) {
}

dTLS*
dTLS::init() {
    if (!_super::init()) return nil;
#if defined (IS_WIN)
    _index = ::TlsAlloc();
    if (TLS_OUT_OF_INDEXES == _index) return nil;
#elif defined (IS_MAC)
    ::pthread_key_create(&_index, nil);
    if (0 == _index) return nil;
#endif
    setValue(nil);
    return this;
}

void
dTLS::dealloc() {
#if defined (IS_WIN)
    ::TlsFree(_index);
#elif defined (IS_MAC)
    ::pthread_key_delete(_index);
#endif
    _super::dealloc();
}

void*
dTLS::value() const {
    void* result = nil;
#if defined (IS_WIN)
    result = ::TlsGetValue(_index);
    if ((!result) && (ERROR_SUCCESS != ::GetLastError()))
        throw new ExGetValueFalse;
#elif defined (IS_MAC)
    result = ::pthread_getspecific(_index);
#endif
    return result;
}

void
dTLS::setValue(void* val) {
#if defined (IS_WIN)
    ::TlsSetValue(_index, val);
#elif defined (IS_MAC)
    ::pthread_setspecific(_index, val);
#endif
}


CL_CLASS_DEF(dCriticalSection, dCocoa);

dCriticalSection::dCriticalSection() {
#if defined (IS_WIN)
    ZERO_MEMORY_STRUCT(_criticalSection);
#elif defined (IS_MAC)
    ZERO_MEMORY_STRUCT(_mutex);
    _lockCount = 0;
    _ownThread = 0;
#endif
}

dCriticalSection*
dCriticalSection::init() {
    if (!_super::init()) return nil;

#if defined (IS_WIN)
    ::InitializeCriticalSection(&_criticalSection);
#elif defined (IS_MAC)
    ::pthread_mutex_init(&_mutex, nil);
#endif
    return this;
}

void
dCriticalSection::dealloc() {
#if defined (IS_WIN)
    ::DeleteCriticalSection(&_criticalSection);
#elif defined (IS_MAC)
    ::pthread_mutex_destroy(&_mutex);
#endif
    _super::dealloc();
}

void
dCriticalSection::lock() {
#if defined (IS_WIN)
    ::EnterCriticalSection(&_criticalSection);
#elif defined (IS_MAC)
    if (!_ownThread || ::pthread_self() != _ownThread) {
        ::pthread_mutex_lock(&_mutex);
        if (!_ownThread) _ownThread = ::pthread_self();
    }
    ++_lockCount;
#endif
}

void
dCriticalSection::unlock() {
#if defined (IS_WIN)
    ::LeaveCriticalSection(&_criticalSection);
#elif defined (IS_MAC)
    if (::pthread_self() == _ownThread) {
        --_lockCount;
        if (0 == _lockCount) {
            _ownThread = 0;
            ::pthread_mutex_unlock(&_mutex);
        }
    }
#endif
}

bool
dCriticalSection::trylock() {
    bool result = false;

#if defined (IS_WIN)
    if (::TryEnterCriticalSection(&_criticalSection))
        result = true; 
#elif defined (IS_MAC)
    if (!_ownThread || ::pthread_self() != _ownThread) {
        int res = ::pthread_mutex_trylock(&_mutex);
        if (0 == res) {
            if (!_ownThread) _ownThread = ::pthread_self();
        } else {
            return false;
        }
    }
    ++_lockCount;
    result = true;
#endif
    return result;
}

dCriticalSection::StLocker::StLocker(dCriticalSection* resource, bool needWait)
: _resource(resource),
  _wasLocked(false)
{
	CL_THROW_IF_NIL(_resource);

    if (needWait) {
        _resource->lock();
        _wasLocked = true;
    } else {
        _wasLocked = _resource->trylock();
    }
}

dCriticalSection::StLocker::~StLocker() {
    if (_resource) _resource->unlock();
}

bool
dCriticalSection::StLocker::wasLocked() const {
    return _wasLocked;
}