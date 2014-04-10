/*
** $Id: 2009/04/09, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dAssembly.h"


dAssembly::dAssembly(const char* moduleName, dInteger ver)
: _version(ver) {

#if defined(IS_WIN)
    strcpy_s(_name, kFixLen, moduleName);
    _moduleHandle = nil;
#elif defined(IS_MAC)
    strcpy(_name, moduleName);
#endif

    _classes = new _dClassSet;
}

dAssembly::~dAssembly() {
    SAFE_DELETE(_classes);
}

const char*
dAssembly::name() const {
    return _name;
}

#if defined(IS_WIN)
HINSTANCE
dAssembly::moduleHandle() const {
    return _moduleHandle;
}

void
dAssembly::setModuleHandle(HINSTANCE inst) {
    _moduleHandle = inst;
}
#endif

const dClass*
dAssembly::classByName(const char* clsName) const {
    _dClassSet::Iterator iter(_classes);
    while (iter.next())
        if (!strcmp(iter.object()->name(), clsName))
            return iter.object();
    return nil;
}

void
dAssembly::addClass(const dClass* cls) {
    _classes->addObject(cls);
}