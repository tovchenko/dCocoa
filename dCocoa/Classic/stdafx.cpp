/*
** $Id: 2009/02/07, Taras Tovchenko $
** dCocoa.pch will be the pre-compiled header
** See Copyright Notice in dCocoa.h
*/

#include "stdafx.h"

CL_ASSEMBLY_DEF(dCocoa, 0)


// define instance of special constant for null object
const dByte __kNull = kNotDefined;

// dGraphics2D.h defining constants
const dPoint CL::dZeroPoint = { 0, 0 };
const dSize CL::dZeroSize = { 0, 0 };
const dRect CL::dZeroRect = { { 0, 0 }, { 0, 0 } };
