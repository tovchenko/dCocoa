/*
** $Id: 2009/03/16, Taras Tovchenko $
** Class-object which uses for keep meta-info about every d-class
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DCLASS_H
#define _COCOA_DCLASS_H



BEGIN_NAMESPACE_CL
class dObject;
class dAssembly;
class dArray;

// selectors support. dSEL - must be always a constant string,
// its not copy, copy only pointer to it. For this puspose use
// special macroses - dselector, METHOD_DEFINE.
#define dselector(methodName)                       ((CL::dSEL)#methodName)
#define CL_METHOD_DECL(methodName)                  virtual CL::dObject* methodName(CL::dArray* __args);
#define CL_METHOD_DEF(className, methodName)    \
    static CL::dClass::__SelectorInstaller __##className##_dMethod_##methodName(    \
        className::staticClass(),    \
        dselector(methodName),    \
        (CL::dObjectMethodPtr)&className::methodName);    \
    CL::dObject* className::methodName(CL::dArray* __args)

#if defined (_DEBUG)
class ExSelectorWrongArgType : public CL::CException { };
class ExSelectorWrongArgNo : public CL::CException { };

template <typename T>
inline T* __dbg_dtype_check(dArray* objects, dInteger index) {
    if (index >= objects->count())
        throw new ExSelectorWrongArgNo;

    T* dobj = dobj_cast<T>(objects->objectAtIndex(index));
    if (!dobj)
        throw new ExSelectorWrongArgType;

    return dobj;
}

    #define CL_ARG(dType, index)               CL::__dbg_dtype_check<dType>(__args, index)
#else
    #define CL_ARG(dType, index)               (dType*)__args->objectAtIndex(index)
#endif

typedef const char*     dSEL;
typedef dObject* (dObject::*dObjectMethodPtr)(dArray* __args);

bool DLL_PUBLIC   dSelectorsAreEqual(dSEL s1, dSEL s2);



#define dprop(propName)             __prop_##propName
#define CL_PROP_INIT(propName)      dprop(propName)(_this())
#define CL_PROP_RELEASE(propName)   CL_RELEASE(__propValue_##propName)

#define CL_PROP_DEF(propType, propName)    \
	private:    \
		propType   __propValue_##propName;    \
	public:    \
		struct _PropSetter_##propName {    \
			_PropSetter_##propName(_self* obj) : _object(obj) {    \
			_object->__propValue_##propName = nil;    \
		}    \
		_PropSetter_##propName& operator=(propType rf) {    \
			_object->_set_##propName(rf);    \
			return *this;    \
		}    \
		operator propType() {    \
			return _object->_get_##propName();    \
		}    \
		propType operator->() const {    \
			return _object->__propValue_##propName;    \
		}    \
		_self* _object;    \
	} dprop(propName);    \
	virtual void _set_##propName(propType propValue) {    \
		CL_SWITCH_LINKS(__propValue_##propName, propValue);    \
	}    \
	virtual propType _get_##propName() const {    \
		return __propValue_##propName;    \
	}


class DLL_PUBLIC dClass {
friend class dInvocation; // _methodPtrForSelector
friend class __SelectorInstaller; // _addMethod
public:
    // helper class
    class __SelectorInstaller {
    public:
        __SelectorInstaller(dClass* cls, dSEL sel, dObjectMethodPtr impl) {
            cls->_addMethod(sel, impl);
        }
    };
    // callbacks
    typedef dObject*    (PASCAL *dAllocObjectCB)();
    typedef dClass*     (PASCAL *dSuperClassCB)();
    typedef void        (PASCAL *dStaticCallCB)();
    // ctor / dtor
    dClass(
        dAssembly* assembly,
        const char* className,
        dUInteger size,
        dInteger version,
        dAllocObjectCB allocCB,
        dSuperClassCB superCB,
        dStaticCallCB ctorCB = nil,
        dStaticCallCB dtorCB = nil);
    ~dClass();
    // allocating
    dObject*                    alloc() const;
    // attributes
    dInteger                    version() const;
    void                        setVersion(dUInteger ver);
    const char*                 name() const;
    // info
    bool                        isSubclassOfClass(dClass* cls) const;
    bool                        respondsToSelector(dSEL sel) const { return _methodPtrForSelector(sel) != nil; }

protected:
    void                        _addMethod(dSEL sel, dObjectMethodPtr impl);
    dObjectMethodPtr            _methodPtrForSelector(dSEL sel) const;

    char                        _className[kFixLen];
    dUInteger                   _size;
    dInteger                    _version;
    dAllocObjectCB              _allocCB;
    dSuperClassCB               _superCB;

    dStaticCallCB               _dtorCB;

    class _dMethodsDict;
    _dMethodsDict*              _methods;

private:
    // forbidden
    dClass(const dClass& rf);
    dClass& operator=(const dClass& rf);
};

END_NAMESPACE_CL

#endif // _COCOA_DCLASS_H