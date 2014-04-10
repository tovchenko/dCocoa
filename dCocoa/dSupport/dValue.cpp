/*
** $Id: 2009/05/06, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dValue.h"


CL_CLASS_DEF(dValue, dCocoa)


dValue::dValue() {
    ZERO_MEMORY_ARRAY(_buffer);
}

dValue*
dValue::valueWithInt(dInteger intVal) {
    dValue* dv = alloc();
    return (dv && dv->initWithInt(intVal)) ? dv->autorelease() : nil;
}

dValue*
dValue::valueWithFloat(float floatVal) {
    dValue* dv = alloc();
    return (dv && dv->initWithFloat(floatVal)) ? dv->autorelease() : nil;
}

dValue*
dValue::valueWithDouble(double doubleVal) {
    dValue* dv = alloc();
    return (dv && dv->initWithDouble(doubleVal)) ? dv->autorelease() : nil;
}

dValue*
dValue::valueWithPtr(void* ptrVal) {
    dValue* dv = alloc();
    return (dv && dv->initWithPtr(ptrVal)) ? dv->autorelease() : nil;
}

dValue*
dValue::valueWithBool(bool boolVal) {
	dValue* dv = alloc();
	return (dv && dv->initWithBool(boolVal)) ? dv->autorelease() : nil;
}

dValue*
dValue::valueWithPoint(const dPoint& pointVal) {
	dValue* dv = alloc();
	return (dv && dv->initWithPoint(pointVal)) ? dv->autorelease() : nil;
}

dValue*
dValue::valueWithSize(const dSize& sizeVal) {
	dValue* dv = alloc();
	return (dv && dv->initWithSize(sizeVal)) ? dv->autorelease() : nil;
}

dValue*
dValue::valueWithRect(const dRect& rectVal) {
	dValue* dv = alloc();
	return (dv && dv->initWithRect(rectVal)) ? dv->autorelease() : nil;
}

dValue*
dValue::initWithInt(dInteger intVal) {
    if (!_super::init()) return nil;
    setIntValue(intVal);
    return this;
}

dValue*
dValue::initWithFloat(float floatVal) {
    if (!_super::init()) return nil;
    setFloatValue(floatVal);
    return this;
}

dValue*
dValue::initWithDouble(double doubleVal) {
    if (!_super::init()) return nil;
    setDoubleValue(doubleVal);
    return this;
}

dValue*
dValue::initWithPtr(void* ptrVal) {
    if (!_super::init()) return nil;
    setPtrValue(ptrVal);
    return this;
}

dValue*
dValue::initWithBool(bool boolVal) {
	if (!_super::init()) return nil;
	setBoolValue(boolVal);
	return this;
}

dValue*
dValue::initWithPoint(const dPoint& pointVal) {
	if (!_super::init()) return nil;
	setPointValue(pointVal);
	return this;
}

dValue*
dValue::initWithSize(const dSize& sizeVal) {
	if (!_super::init()) return nil;
	setSizeValue(sizeVal);
	return this;
}

dValue*
dValue::initWithRect(const dRect& rectVal) {
	if (!_super::init()) return nil;
	setRectValue(rectVal);
	return this;
}

dInteger
dValue::intValue() const {
    if (kTypeInt != _type)
        throw new ExIncorrectType;
    return *(dInteger*)_buffer;
}

float
dValue::floatValue() const {
    if (kTypeFloat != _type)
        throw new ExIncorrectType;
    return *(float*)_buffer;
}

double
dValue::doubleValue() const {
    if (kTypeDouble != _type)
        throw new ExIncorrectType;
    return *(double*)_buffer;
}

void*
dValue::ptrValue() const {
    if (kTypePtr != _type)
        throw new ExIncorrectType;
    return *(void**)_buffer;
}

bool
dValue::boolValue() const {
	if (kTypeBool != _type)
		throw new ExIncorrectType;
	return *(bool*)_buffer;
}

dPoint
dValue::pointValue() const {
	if (kTypePoint != _type)
		throw new ExIncorrectType;
	return *(dPoint*)_buffer;
}

dSize
dValue::sizeValue() const {
	if (kTypeSize != _type)
		throw new ExIncorrectType;
	return *(dSize*)_buffer;
}

dRect
dValue::rectValue() const {
	if (kTypeRect != _type)
		throw new ExIncorrectType;
	return *(dRect*)_buffer;
}

void
dValue::setIntValue(dInteger intVal) {
    _type = kTypeInt;
    *(dInteger*)_buffer = intVal;
}

void
dValue::setFloatValue(float floatVal) {
    _type = kTypeFloat;
    *(float*)_buffer = floatVal;
}

void
dValue::setDoubleValue(double doubleVal) {
    _type = kTypeDouble;
    *(double*)_buffer = doubleVal;
}

void
dValue::setPtrValue(void* ptrVal) {
    _type = kTypePtr;
    *(void**)_buffer = ptrVal;
}

void
dValue::setBoolValue(bool boolVal) {
	_type = kTypeBool;
	*(bool*)_buffer = boolVal;
}

void
dValue::setPointValue(const dPoint& pointVal) {
	_type = kTypePoint;
	*(dPoint*)_buffer = pointVal;
}

void
dValue::setSizeValue(const dSize& sizeVal) {
	_type = kTypeSize;
	*(dSize*)_buffer = sizeVal;
}

void
dValue::setRectValue(const dRect& rectVal) {
	_type = kTypeRect;
	*(dRect*)_buffer = rectVal;
}

dInteger
dValue::hash() const {
    return uALGO::hashFromByteSequence(_buffer, sizeof(_buffer));
}

bool
dValue::isEqual(const dObject* otherObject) const {
    const dValue* otherVal = (dValue*)(otherObject);
    if (_type != otherVal->_type) return false;
    switch (_type) {
        case kTypeInt: if (intValue() == otherVal->intValue()) return true; break;
        case kTypeFloat: if (floatValue() == otherVal->floatValue()) return true; break;
        case kTypeDouble: if (doubleValue() == otherVal->doubleValue()) return true; break;
        case kTypePtr: if (ptrValue() == otherVal->ptrValue()) return true; break;
		case kTypeBool: if (boolValue() == otherVal->boolValue()) return true; break;
		case kTypePoint: {
			dPoint selfPt = pointValue();
			dPoint otherPt = otherVal->pointValue();
			if (selfPt.x == otherPt.x && selfPt.y == otherPt.y)
				return true;
			break;
		}
		case kTypeSize: {
			dSize selfSz = sizeValue();
			dSize otherSz = otherVal->sizeValue();
			if (selfSz.width == otherSz.width && selfSz.height == otherSz.height)
				return true;
			break;
		}
		case kTypeRect: {
			dRect selfR = rectValue();
			dRect otherR = otherVal->rectValue();
			if (selfR.origin.x == otherR.origin.x && selfR.origin.y == otherR.origin.y &&
				selfR.size.width == otherR.size.width && selfR.size.height == otherR.size.height)
				return true;
			break;
		}
    }

    return false;
}