/*
** $Id: 2009/04/09, Taras Tovchenko $
** Keep info about module, such as list of d-classes, name and handels
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DASSEMBLY_H
#define _COCOA_DASSEMBLY_H

#include "CSetT.h"
#include "CAlgorithms.h"

#if defined (IS_WIN)
    #include <windows.h>
#endif


// MAIN RULE: every module which use dCocoa must define a it's own Assembly,
// than all classes at this module will be added to it's assembly. Use following
// macroses for declare and define Assembly. For library modules need write the
// macro CL_ASSEMBLY_DECL in a private header (which not exported with library,
// for example in VC use stdafx.h and stdafx.cpp)

#define __DASSEMBLY(name)       __assembly##name
#define dassembly(name)         __DASSEMBLY(name)()

#define dAssemblyStr(name)      _TO_STR(__DASSEMBLY(name))

#define CL_ASSEMBLY_DECL(name, api_spec)    \
    extern "C" api_spec CL::dAssembly* dassembly(name);

#define CL_ASSEMBLY_DECL_NO_EXPORT(name)    \
    extern "C" CL::dAssembly* dassembly(name);

#define CL_ASSEMBLY_DEF(name, version)    \
    CL::dAssembly* dassembly(name) {    \
        static CL::dAssembly _assembly(_TO_STR(name), version);    \
        return &_assembly;    \
    }



BEGIN_NAMESPACE_CL


template <typename TYPE>
class dClassHashEqualPolicyT {
public:
    static dInteger     hashValueForKey(const TYPE* o) { return uALGO::hashFromString((*o)->name()); }
    static bool         isEqualKeys(const TYPE* o1, const TYPE* o2, void* context) { return !strcmp((*o1)->name(), (*o2)->name()); }
};

typedef CSetT<const dClass*,
              dClassHashEqualPolicyT,
              const dClass*> _dClassSet;


class DLL_PUBLIC dAssembly {
friend class dClass; // addClass
public:
    dAssembly(const char* moduleName, dInteger ver = 0);
    ~dAssembly();
    // attributes
    const char*                 name() const;
#if defined (IS_WIN)
    HINSTANCE                   moduleHandle() const;
    void                        setModuleHandle(HINSTANCE inst);
#endif
    // getting symbols
    const dClass*               classByName(const char* clsName) const;

protected:
    // adding symbols
    void                        addClass(const dClass* cls);

    char                        _name[kFixLen];
    dInteger                    _version;
    _dClassSet*                 _classes;
#if defined (IS_WIN)
    HINSTANCE                   _moduleHandle;
#endif

private:
    // forbidden
    dAssembly(const dAssembly& rf);
    dAssembly& operator=(const dAssembly& rf);
};

END_NAMESPACE_CL

#endif // _COCOA_DASSEMBLY_H