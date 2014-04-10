/*
** $Id: 2009/03/16, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dObject.h"

#include "dInvocation.h"


CL_ABSTRACT_DEF_WITH_NIL_SUPER(dObject, dCocoa)


dObject::dObject()
: _retainCount(1) {
}

dObject*
dObject::init() {
    // don't call _super::init(), because will be recursion
    return this;
}

void
dObject::dealloc() {
    // don't call _super::dealloc() because will be recursion
}

void
dObject::release() const {
    if (0 == --_retainCount) {
        const_cast<dObject*>(this)->dealloc();
        delete this;
    }
}

dInteger
dObject::hash() const {
    return ((long long int)this) >> 2;
}

bool
dObject::isEqual(const dObject* otherObject) const {
    return (void*)this == (void*)otherObject;
}

dObject*
dObject::invokeSelector(dSEL sel, ...) {
    va_list argList;
    va_start(argList, sel);

    dObject* retValue = invokeSelectorWithVAList(sel, argList);

    va_end(argList);
    return retValue;
}

dObject*
dObject::invokeSelectorWithVAList(dSEL sel, va_list argList) {
    dobj_ptr<dInvocation>   inv(dInvocation::alloc()->initWithVAList(this, sel, argList));
    inv->invoke();
    return inv->returnValue();
}


CL_CLASS_DEF_AS_SINGLETON(dNull, dCocoa)