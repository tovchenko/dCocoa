/*
** $Id: 2009/02/07, Taras Tovchenko $
** Include file for standard system include files,
** or project specific include files that are used frequently, but
** are changed infrequently
** See Copyright Notice in dCocoa.h
*/

#ifndef _dCocoa_StdAfx_H_
#define _dCocoa_StdAfx_H_


#include "_Private.h"

#if defined(IS_WIN)
    #include "targetver.h"
#endif


#include "dByteOrder.h"
#include "CException.h"
#include "dRange.h"
#include "dGraphics2D.h"
#include "dObject.h"
#include "dAutoPool.h"
#include "dAssembly.h"


CL_ASSEMBLY_DECL(dCocoa, DLL_PUBLIC)

BEGIN_NAMESPACE_CL
END_NAMESPACE_CL
USING_NAMESPACE_CL

#endif // _dCocoa_StdAfx_H_