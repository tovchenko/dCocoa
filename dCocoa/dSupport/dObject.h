/*
** $Id: 2009/03/16, Taras Tovchenko $
** Base class which can have d-meta-info for your class
** which will be derived at dObject
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DOBJECT_H
#define _COCOA_DOBJECT_H

#include "dClass.h"


#define CL_ABSTRACT_DECL(selfClass, superClass)    \
    __GENERIC_METHODS_DECL(selfClass, superClass)

#define CL_ABSTRACT_DEF(selfClass, assemblyName)    \
    CL_ABSTRACT_DEF_WITH_STATIC_CTORDTOR(selfClass, assemblyName, nil, nil)

#define CL_CLASS_DECL(selfClass, superClass)    \
    __GENERIC_METHODS_DECL(selfClass, superClass)    \
    public:    \
        static selfClass* PASCAL    alloc();

#define CL_CLASS_DECL_AS_SINGLETON(selfClass, superClass)    \
    __GENERIC_METHODS_DECL(selfClass, superClass)    \
    public:    \
        static selfClass* PASCAL    singleton();    \
    private:    \
        static selfClass* PASCAL    alloc();

#define CL_CLASS_DEF(selfClass, assemblyName)    \
    CL_CLASS_DEF_WITH_STATIC_CTORDTOR(selfClass, assemblyName, nil, nil)

#define CL_CLASS_DEF_AS_SINGLETON(selfClass, assemblyName)    \
    CL_CLASS_DEF_WITH_STATIC_CTORDTOR_AS_SINGLETON(selfClass, assemblyName, nil, nil)

#define CL_ABSTRACT_DEF_WITH_STATIC_CTORDTOR(selfClass, assemblyName, ctorCB, dtorCB)    \
    __GENERIC_METHODS_DEFS(selfClass, assemblyName, 0, nil, ctorCB, dtorCB)

#define CL_CLASS_DEF_WITH_STATIC_CTORDTOR(selfClass, assemblyName, ctorCB, dtorCB)    \
    __GENERIC_METHODS_DEFS(selfClass, assemblyName, 0, &selfClass::alloc, ctorCB, dtorCB)    \
    selfClass* PASCAL           selfClass::alloc() { return new selfClass; }

#define CL_CLASS_DEF_WITH_STATIC_CTORDTOR_AS_SINGLETON(selfClass, assemblyName, ctorCB, dtorCB)    \
    __GENERIC_METHODS_DEFS(selfClass, assemblyName, 0, &selfClass::singleton, ctorCB, dtorCB)    \
    selfClass* PASCAL           selfClass::singleton() {    \
        static selfClass* instance = nil;    \
        if (!instance) {    \
            instance = new selfClass;    \
            instance->init();    \
            CL::dAutoPool::topPool()->addObject(instance);    \
        }    \
        return instance;    \
    }

#define CL_CLASS_TEMPLATE(selfClass, superClass)    \
    public:    \
        static CL::dClass* PASCAL  staticClass() { return _superClass(); }    \
        virtual CL::dClass*        runtimeClass() const { return selfClass::staticClass(); }    \
        static selfClass* PASCAL    alloc() { return new selfClass; }    \
        selfClass*                  retain() { ++_retainCount; return this; }    \
        const selfClass*            const_retain() const { return const_cast<selfClass*>(this)->retain(); }    \
        selfClass*                  autorelease() const { CL::dAutoPool::currentPool()->addObject(this); return const_cast<selfClass*>(this); }    \
    protected:    \
        static CL::dClass* PASCAL  _superClass() { return superClass::staticClass(); }

#define CL_ABSTRACT_DEF_WITH_NIL_SUPER(selfClass, assemblyName)    \
    __GENERIC_METHODS_DEFS_WITHOUT_SUPER(selfClass, assemblyName, 0, nil, nil, nil)    \
    CL::dClass* PASCAL         selfClass::_superClass() { return nil; }

#define __GENERIC_METHODS_DECL(selfClass, superClass)    \
    private:    \
        typedef selfClass   _self;    \
        typedef superClass  _super;    \
        _self*                      _this() const { return const_cast<_self*>(this); }    \
    public:    \
        static CL::dClass* PASCAL  staticClass();    \
        virtual CL::dClass*        runtimeClass() const;    \
        selfClass*                  retain();    \
        const selfClass*            const_retain() const;    \
        selfClass*                  autorelease() const;    \
    protected:    \
        static CL::dClass* PASCAL  _superClass();    \
        virtual ~selfClass();    \
        static const CL::dClass    __dClass##selfClass;

#define __GENERIC_METHODS_DEFS(selfClass, assemblyName, ver, allocCB, staticCtorCB, staticDtorCB)    \
    __GENERIC_METHODS_DEFS_WITHOUT_SUPER(selfClass, assemblyName, ver, allocCB, staticCtorCB, staticDtorCB)    \
    CL::dClass* PASCAL         selfClass::_superClass() { return _super::staticClass(); }


#define __GENERIC_METHODS_DEFS_WITHOUT_SUPER(selfClass, assemblyName, ver, allocCB, staticCtorCB, staticDtorCB)    \
    CL::dClass* PASCAL         selfClass::staticClass() { return const_cast<CL::dClass*>(&__dClass##selfClass); }    \
    CL::dClass*                selfClass::runtimeClass() const { return selfClass::staticClass(); }    \
    selfClass*                  selfClass::retain() { ++_retainCount; return this; }    \
    const selfClass*            selfClass::const_retain() const { return const_cast<selfClass*>(this)->retain(); }    \
    selfClass*                  selfClass::autorelease() const { CL::dAutoPool::currentPool()->addObject(this); return const_cast<selfClass*>(this); }    \
    selfClass::~selfClass() { }    \
    const CL::dClass selfClass::__dClass##selfClass(    \
        dassembly(assemblyName),    \
        (#selfClass),    \
        sizeof(selfClass),    \
        (ver),    \
        reinterpret_cast<CL::dClass::dAllocObjectCB>(allocCB),    \
        &selfClass::_superClass,    \
        reinterpret_cast<CL::dClass::dStaticCallCB>(staticCtorCB),    \
        reinterpret_cast<CL::dClass::dStaticCallCB>(staticDtorCB));


#define CL_RETAIN(p)                    { if (p) { reinterpret_cast<CL::dObject*>(p)->retain(); } }
#define CL_RELEASE(p)                   { if (p) { reinterpret_cast<CL::dObject*>(p)->release(); (p) = nil; } }
#define CL_SWITCH_LINKS(dst, src)       { CL_RETAIN(src); CL_RELEASE(dst); dst = src; }
#define CL_NEW(className)               static_cast<className*>(className::alloc()->init())


#define isKindOfClass(className)        runtimeClass()->isSubclassOfClass(className::staticClass())
#define isMemberOfClass(className)      runtimeClass() == className::staticClass()

#define CL_NULL(castType)				((castType*)CL::dNull::singleton())



BEGIN_NAMESPACE_CL

class DLL_PUBLIC dObject {
CL_ABSTRACT_DECL(dObject, dObject)
public:
    // initializations
    virtual dObject*            init();
    // memory management
    // alloc() and retain() already defined in macros
    void                        release() const;
    // hash & equal
    virtual dInteger            hash() const;
    virtual bool                isEqual(const dObject* otherObject) const;
    // messaging system
    bool                        respondsToSelector(dSEL sel) const { return runtimeClass()->respondsToSelector(sel); }
    dObject*                    invokeSelector(dSEL sel, ...);
    dObject*                    invokeSelectorWithVAList(dSEL sel, va_list argList);

protected:
    dObject();
    virtual void                dealloc();

    mutable dInteger            _retainCount;
};


// modifed std::auto_ptr for support
// d-object's memory management
template<typename _Tp1>
struct dobj_ptr_ref {
	_Tp1* _M_ptr;
	
	explicit dobj_ptr_ref(_Tp1* __p): _M_ptr(__p) { }
};

template<typename _Tp>
class dobj_ptr {
private:
	_Tp* _M_ptr;
	
public:
	typedef _Tp element_type;
	explicit dobj_ptr(element_type* __p = 0) throw() : _M_ptr(__p) { }
	dobj_ptr(dobj_ptr& __a) throw() : _M_ptr(__a.release()) { }
	template<typename _Tp1> dobj_ptr(dobj_ptr<_Tp1>& __a) throw() : _M_ptr(__a.release()) { }
	dobj_ptr& operator=(dobj_ptr& __a) throw() {
		reset(__a.release());
		return *this;
	}
	
	template<typename _Tp1> dobj_ptr& operator=(dobj_ptr<_Tp1>& __a) throw() {
		reset(__a.release());
		return *this;
	}
	
	~dobj_ptr() { CL_RELEASE(_M_ptr); }
	
	element_type& operator*() const throw() {
		assert(_M_ptr != 0);
		return *_M_ptr;
	}
	
	element_type* operator->() const throw() {
		assert(_M_ptr != 0);
		return _M_ptr;
	}
	
	element_type* get() const throw() { return _M_ptr; }
	element_type* release() throw() {
		element_type* __tmp = _M_ptr;
		_M_ptr = 0;
		return __tmp;
	}
	
	void reset(element_type* __p = 0) throw() {
		if (__p != _M_ptr) {
			CL_RELEASE(_M_ptr);
			_M_ptr = __p;
		}
	}
	
	dobj_ptr(dobj_ptr_ref<element_type> __ref) throw()
	: _M_ptr(__ref._M_ptr) { }
	
	dobj_ptr& operator=(dobj_ptr_ref<element_type> __ref) throw() {
		if (__ref._M_ptr != this->get()) {
			CL_RELEASE(_M_ptr);
			_M_ptr = __ref._M_ptr;
		}
		return *this;
	}
	
	template<typename _Tp1> operator dobj_ptr_ref<_Tp1>() throw() {
		return dobj_ptr_ref<_Tp1>(this->release());
	}
	
	template<typename _Tp1> operator dobj_ptr<_Tp1>() throw() {
		return dobj_ptr<_Tp1>(this->release());
	}
};


// dynamic casting for d-object's types
template <typename T>
inline T* dobj_cast(CL::dObject* obj) {
    if (obj && obj->isKindOfClass(T)) return (T*)obj;
    return nil;
}

template <typename T>
inline const T* dobj_cast(const CL::dObject* obj) {
    if (obj && obj->isKindOfClass(T)) return (const T*)obj;
    return nil;
}

// object used to represent null values in collection objects (which don’t allow nil values)
class dNull : public dObject {
CL_CLASS_DECL_AS_SINGLETON(dNull, dObject)
};

END_NAMESPACE_CL

#endif // _COCOA_DOBJECT_H