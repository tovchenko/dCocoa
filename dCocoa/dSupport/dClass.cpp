/*
** $Id: 2009/03/16, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dClass.h"

#include "CDictT.h"

#include <string.h>


BEGIN_NAMESPACE_CL

bool
dSelectorsAreEqual(dSEL s1, dSEL s2) {
    return s1 == s2 || !strcmp(s1, s2);
}

END_NAMESPACE_CL


template <typename OBJ_TYPE, typename KEY_TYPE>
class _SelectorHashEqualPolicyT {
public:
    static dInteger     hashValueForKey(const KEY_TYPE* k) { return uALGO::hashFromString(*k); }
    static bool         isEqualKeys(const KEY_TYPE* k1, const KEY_TYPE* k2, void* context) { return dSelectorsAreEqual(*k1, *k2); }
    static bool         isEqualObjects(const OBJ_TYPE* o1, const OBJ_TYPE* o2, void* context) { return *o1 == *o2; }
};

class dClass::_dMethodsDict : public CDictT<
                                        dObjectMethodPtr, dSEL,
                                        _SelectorHashEqualPolicyT,
                                        dObjectMethodPtr, dSEL> {
};


dClass::dClass(
       dAssembly* assembly,
       const char* className,
       dUInteger size,
       dInteger version,
       dAllocObjectCB allocCB,
       dSuperClassCB superCB,
       dStaticCallCB ctorCB,
       dStaticCallCB dtorCB)
: _size(size),
  _version(version),
  _allocCB(allocCB),
  _superCB(superCB),
  _dtorCB(dtorCB),
  _methods(nil) {

#if defined (IS_WIN)
    strcpy_s(_className, 255, className);
#elif defined (IS_MAC)
    strcpy(_className, className);
#endif

    assembly->addClass(this);

    if (ctorCB) (*ctorCB)();
}

dClass::~dClass() {
    if (_dtorCB) (*_dtorCB)();

    SAFE_DELETE(_methods);
}

dObject*
dClass::alloc() const {
    return (*_allocCB)();
}

dInteger
dClass::version() const {
    return _version;
}

void
dClass::setVersion(dUInteger ver) {
    _version = ver;
}

const char*
dClass::name() const {
    return _className;
}

bool
dClass::isSubclassOfClass(dClass* cls) const {
    if (!cls) return false;

    dClass* selfClass = const_cast<dClass*>(this);
    do {
        if (selfClass == cls) return true;
    } while ((selfClass = (*selfClass->_superCB)()));

    return false;
}

void
dClass::_addMethod(dSEL sel, dObjectMethodPtr impl) {
    if (!_methods) _methods = new _dMethodsDict;
    _methods->setObjectForKey(impl, sel);
}

dObjectMethodPtr
dClass::_methodPtrForSelector(dSEL sel) const {
	CL_THROW_IF_NIL(sel);
    if (_methods) {
        dClass* cls = const_cast<dClass*>(this);
        do {
            try {
                if (cls->_methods)
                    return cls->_methods->objectForKey(sel);
            } catch (_dMethodsDict::ExDictObjectNotFound* e) {
                SAFE_DELETE(e);
            }
        } while ((cls = (*cls->_superCB)()));
    }

    return nil;
}