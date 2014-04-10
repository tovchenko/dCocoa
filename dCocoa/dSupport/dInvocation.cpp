/*
** $Id: 2009/04/04, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dInvocation.h"

#include "dArray.h"


CL_CLASS_DEF(dInvocation, dCocoa)


dInvocation::dInvocation()
: _target(nil),
  _selector(nil),
  _args(nil),
  _returnValue(nil),
  _methodCache(nil),
  _targetCache(nil) {
}

dInvocation*
dInvocation::initWithParams(dObject* target, dSEL sel, ...) {
    va_list dobjsList;
    va_start(dobjsList, sel);

    dInvocation* tmp = initWithVAList(target, sel, dobjsList);

    va_end(dobjsList);
    return tmp;
}

dInvocation*
dInvocation::initWithVAList(dObject* target, dSEL sel, va_list argList) {
    if (!_super::init()) return nil;
    if (!sel) {
        release();
		throw new ExAssert;
    }

    if (target) _target = target->retain();
    _selector = sel;

    dObject* object = va_arg(argList, dObject*);
    if (object) _args = CL_NEW(dArray);
    while (object) {
        _args->addObject(object);
        object = va_arg(argList, dObject*);
    }

    return this;
}

void
dInvocation::invokeWithTarget(dObject* target) {
    if (!target) return;
    
    if (!_targetCache || _targetCache != target) {
        _targetCache = target;
        _methodCache = _targetCache->runtimeClass()->_methodPtrForSelector(_selector);
    }
    
    if (_methodCache) {
        CL_RELEASE(_returnValue);
        _returnValue = (_targetCache->*_methodCache)(_args);
        CL_RETAIN(_returnValue);
    } else {
        // TODO: target->forwardInvocation(this);
    }
}

void
dInvocation::invoke() {
    invokeWithTarget(_target);
}

dObject*
dInvocation::returnValue() const {
    return _returnValue;
}


void
dInvocation::dealloc() {
    CL_RELEASE(_target);
    CL_RELEASE(_returnValue);
    CL_RELEASE(_args);

    _super::dealloc();
}