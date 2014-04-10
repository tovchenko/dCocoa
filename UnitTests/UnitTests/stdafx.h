/*
** $Id: 2009/02/07, Taras Tovchenko $
** Unit Tests, dUnitTests.pch will be the pre-compiled header
** See Copyright Notice in dCocoa.h
*/

#ifndef _UnitTests_H
#define _UnitTests_H

#include <stdio.h>
#include <dCocoa/dCocoa.h>
#include <dCocoa/tchar.h>
USING_NAMESPACE_CL


CL_ASSEMBLY_DECL_NO_EXPORT(dUnitTests)


#include "TestData.h"

#ifdef IS_WIN
    typedef __int64         TTime;
#else
    typedef unsigned long   TTime;
#endif



#define TEST_MSG_INFO(_var_, _desc_)    static const TCHAR* const _var_ = _T(_desc_);
void     testFail(const TCHAR* msg);

//inline TTime
//getTics() {
//    __asm__ rdtsc
//}

#endif // _UnitTests_H