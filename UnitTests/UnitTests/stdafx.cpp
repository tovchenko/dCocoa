/*
** $Id: 2009/02/07, Taras Tovchenko $
** Unit Tests, dUnitTests.pch will be the pre-compiled header
** See Copyright Notice in dCocoa.h
*/

#include "stdafx.h"


CL_ASSEMBLY_DEF(dUnitTests, 0)

void
testFail(const TCHAR* msg) {
	_tprintf(_T("ERROR! Test Fail: %s"), msg);
}