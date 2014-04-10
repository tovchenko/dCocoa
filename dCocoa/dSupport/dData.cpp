/*
** $Id: 2009/07/06, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dData.h"
#include "dFileHandle.h"

#include "zlib.h"

#include <memory>

enum { kCompressedSignature = 'zdat' };

#define _BYTES						(kCopy == _ownMode ? (_bytesCopy ? &_bytesCopy->objectAtIndex(0) : nil) : _bytesNoCopy)
#define _FREE_WHEN_DONE_IF_NEEDED	if (kNoCopyWithDelete == _ownMode) { SAFE_DELETE_ARRAY(_bytesNoCopy); } else if (kNoCopyNoDelete == _ownMode) _bytesNoCopy = nil;
#define _DELETE_BYTES				if (kCopy == _ownMode) { SAFE_DELETE(_bytesCopy); } else { _FREE_WHEN_DONE_IF_NEEDED }


CL_CLASS_DEF(dData, dCocoa)

dData::dData()
: _bytesNoCopy(nil),
  _length(0),
  _ownMode(kCopy) {
}

dData*
dData::initWithBytes(const void* bytes, dUInteger length) {
	if (!_super::init()) return nil;
	setBytes(bytes, length, kCopy);
	return this;
}

dData*
dData::initWithBytesNoCopy(void* bytes, dUInteger length, bool deleteWhenDone) {
    if (!_super::init()) return nil;
    setBytes(bytes, length, deleteWhenDone ? kNoCopyWithDelete : kNoCopyNoDelete);
    return this;
}

dData*
dData::initWithData(const dData* data) {
	if (!_super::init()) return nil;
	setBytes(data->bytes(), data->length(), kCopy);
	return this;
}

dData*
dData::initWithContentsOfFile(const dString* path) {
	dFileHandle* file = dFileHandle::fileHandleForReadingAtPath(path);
	dData* data = file->readDataToEndOfFile();
	CL_THROW_IF_NIL(data);
	
	release();
	return data->retain();
}

dData*
dData::dataWithBytes(const void* bytes, dUInteger length) {
	_self* tmp = alloc()->initWithBytes(bytes, length);
	return tmp ? tmp->autorelease() : nil;
}

dData*
dData::dataWithBytesNoCopy(void* bytes, dUInteger length, bool deleteWhenDone) {
	_self* tmp = alloc()->initWithBytesNoCopy(bytes, length, deleteWhenDone);
	return tmp ? tmp->autorelease() : nil;
}

dData*
dData::dataWithData(const dData* data) {
	_self* tmp = alloc()->initWithData(data);
	return tmp ? tmp->autorelease() : nil;
}

dData*
dData::dataWithContentsOfFile(const dString* path) {
	dFileHandle* file = dFileHandle::fileHandleForReadingAtPath(path);
	dData* data = file->readDataToEndOfFile();
	CL_THROW_IF_NIL(data);
	
	return data;
}

void
dData::setBytes(const void* bytes, dUInteger length, OwnMode mode) {
	switch (mode) {
		case kCopy:
            if (kCopy == _ownMode && _bytesCopy) _bytesCopy->setObjectsToRange((const dByte*)bytes, dMakeRange(0, length), true);
            else {
                _FREE_WHEN_DONE_IF_NEEDED
                _bytesCopy = new CArrayT<dByte>((const dByte*)bytes, length);
            }
			break;
		case kNoCopyNoDelete:
		case kNoCopyWithDelete:
            _DELETE_BYTES
			_bytesNoCopy = (dByte*)bytes;
			_length = length;
	}

    _ownMode = mode;
}

void
dData::dealloc() {
	_DELETE_BYTES
	_super::dealloc();
}

const void*
dData::bytes() const {
	return _BYTES;
}

dUInteger
dData::length() const {
	return kCopy == _ownMode ? (_bytesCopy ? _bytesCopy->count() : 0) : _length;
}

dInteger
dData::hash() const {
	return uALGO::hashFromByteSequence(_BYTES, length());
}

bool
dData::isEqual(const dObject* otherObject) const {
	CL_THROW_IF_NIL(otherObject);
	dData* otherData = (dData*)otherObject;
	
	if (this == otherData) return true;
	if (length() == otherData->length())
		return !std::memcmp(_BYTES, otherData->bytes(), length());
	return false;
}

dData*
dData::compressedData() const {
	uLong srcLen = length();
	uLongf destLen = compressBound(srcLen);
	Bytef* dataPtr = (Bytef*)new dByte[destLen + 8];
	dData* data = nil;
	
	if (Z_OK == compress(dataPtr, &destLen, (Bytef*)bytes(), srcLen)) {
		*(dUInteger*)(dataPtr + destLen) = dSwapHostIntToLittle((dUInteger)srcLen);
		*(dUInteger*)(dataPtr + destLen + sizeof(dUInteger)) = dSwapHostIntToLittle(kCompressedSignature);
		data = dData::dataWithBytesNoCopy(dataPtr, (dUInteger)destLen + 8, true);
	}
	
	return data;
}

dData*
dData::uncompressedData() const {
	uLong srcLen = length() - 2 * sizeof(dUInteger);
	dUInteger* signs = (dUInteger*)((Bytef*)bytes() + srcLen);
	
	if (dSwapHostIntToLittle(kCompressedSignature) != signs[1]) return nil;
	uLongf dstLen = dSwapHostIntToLittle(signs[0]);
	
	Bytef* dataPtr = (Bytef*)new dByte[dstLen];
	if (Z_OK == uncompress(dataPtr, &dstLen, (Bytef*)bytes(), srcLen)) {
		return dData::dataWithBytesNoCopy(dataPtr, (dUInteger)dstLen, true);
	}
	
	SAFE_DELETE_ARRAY(dataPtr);
	return nil;
}