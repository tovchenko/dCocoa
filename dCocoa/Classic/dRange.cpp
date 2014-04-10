/*
** $Id: 2009/02/07, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dRange.h"


const dRange CL::dZeroRange = { 0, 0 };

ExRange::ExRange(dInteger wrongIndex)
: _wrongIndex(wrongIndex) {

}

dInteger
ExRange::wrongIndex() const {
    return _wrongIndex;
}
