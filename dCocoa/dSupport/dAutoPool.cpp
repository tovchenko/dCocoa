/*
** $Id: 2009/03/23, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dAutoPool.h"

#include "dArray.h"


CL_CLASS_DEF(dAutoPool, dCocoa)

static dInteger     kObjectsOnPage = 512;

static dAutoPool*	_sCurrentPool = nil;
static dAutoPool*	_sTopPool = nil;


dAutoPool::dAutoPool()
: _pages(nil),
  _parentPool(nil),
  _childPool(nil) {
}

dAutoPool*
dAutoPool::init() {
    if (!_super::init()) return nil;

    dAutoPool* current = currentPool();
    _parentPool = current;
    if (current) current->_childPool = this;
    else _sTopPool = this;
    _childPool = nil;

    _sCurrentPool = this;
    return this;
}

void
dAutoPool::dealloc() {
    CL_RELEASE(_childPool);
    CL_RELEASE(_pages);

    _sCurrentPool = _parentPool;
    if (_parentPool) _parentPool->_childPool = nil;

    _super::dealloc();
}

dAutoPool* PASCAL
dAutoPool::currentPool() {
    return _sCurrentPool;
}

dAutoPool* PASCAL
dAutoPool::topPool() {
    return _sTopPool;
}

void
dAutoPool::addObject(const dObject* obj) {
    if (!_pages) _pages = CL_NEW(dArrayT<dArray*>);

    do {
        if (0 == _pages->count()) break;

        dArray* lastPage = _pages->lastObject();
        dInteger objsOnPage = lastPage->count();
        if (objsOnPage && !(objsOnPage % kObjectsOnPage))
            break;

        lastPage->addObject(const_cast<dObject*>(obj));
        obj->release();
        return;
    } while (false);

    dobj_ptr<dArray> lastPage(CL_NEW(dArray));
    _pages->addObject(lastPage.get());

    lastPage->addObject(const_cast<dObject*>(obj));
    obj->release();
}