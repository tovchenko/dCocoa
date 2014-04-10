/*
** $Id: 2009/02/07, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "CArray.h"
#include "CAlgorithms.h"

#include <cstdarg>
#include <memory>


#define _BUFFER_PTR_AT_INDEX(_index_)   (_buffer + (_index_) * _itemSize)
#define _CHECK_NOT_USED_MEMORY          if (_totalBlocks > 2 * _count) _freeNotUsedMemory()
#define _CHECK_ITEMS_SIZE(_ar_)         CL_THROW_IF_TRUE(_itemSize != _ar_->_itemSize)


// this functions used for removeObjectsFromIndices method
static void     _searchSequentials(dInteger* inNumArray, dRange* outSeqArray, dInteger* inOutCnt);
static void     _calcIndices(dRange* inOutSeqArray, dInteger cnt);



CArray::CArray()
: _itemSize(0),
  _count(0),
  _buffer(nil),
  _totalBlocks(0),
  _growBy(0),
  _isSorted(false) {
}

CArray::~CArray() {
}

void
CArray::initWithArray(const CArray* otherArray) {
	CL_THROW_IF_NIL(otherArray);

    _itemSize = otherArray->_itemSize;
    _count = otherArray->_count;
    _growBy = otherArray->_growBy;

    if (0 == _count) return;
    setCapacity(_count);
    _copyItems(_buffer, otherArray->_buffer, _count);

    _isSorted = otherArray->_isSorted;
}

void
CArray::initWithObjects(dUInteger itemSize, ...) {
    va_list argList;
    _itemSize = itemSize;

    va_start(argList, itemSize);

    void* object = nil;
    while ((object = va_arg(argList, void*)))
        addObject(object);
    va_end(argList);
}

void
CArray::initWithCArray(dUInteger itemSize, const void* objects, dInteger count) {
    if (0 == count) return;

    _itemSize = itemSize;
    _count = count;

    setCapacity(_count);
    _copyItems(_buffer, objects, _count);
}

void
CArray::_dealloc() {
    _destroyItems(_buffer, _count);
    SAFE_DELETE_ARRAY(_buffer);
}

bool
CArray::containsObject(const void* object) const {
    return kNotFound != indexOfObject(object);
}

dInteger
CArray::count() const {
    return _count;
}

void
CArray::getObjects(void* buffer) const {
	CL_THROW_IF_NIL(buffer);
    _copyItems(buffer, _buffer, _count);
}

void
CArray::getObjectsInRange(void* buffer, dRange range) const {
	CL_THROW_IF_NIL(buffer);
    dRangeCheck(range, _count);

    _copyItems(buffer, _BUFFER_PTR_AT_INDEX(range.location), range.length);
}

dInteger
CArray::indexOfObject(const void* object) const {
    return indexOfObjectInRange(object,dMakeRange(0, _count));
}

dInteger
CArray::indexOfObjectInRange(const void* object, dRange range) const {
	CL_THROW_IF_NIL(object);
    dRangeCheck(range, _count);

    if (_count > 0) {
        dInteger index = _isSorted
            ? uALGO::binarySearch(
                _buffer, object,
                range.location, true,
                _count, _itemSize,
                _comparatorCallback, (void*)this)
            : uALGO::linearSearch(
                _buffer, object,
                range.location, true,
                _count, _itemSize,
                _comparatorCallback, (void*)this);

        if (index < range.location + range.length)
            return index;
    }
    return kNotFound;
}

void*
CArray::lastObject() const {
	if (_count <= 0)
		throw new ExRange(0);
		
	return _BUFFER_PTR_AT_INDEX(_count - 1);
}

void*
CArray::objectAtIndex(dInteger index) const {
    if (index >= _count)
        throw new ExRange(index);
    return _BUFFER_PTR_AT_INDEX(index);
}

void*
CArray::objectAtIndexNoException(dInteger index) const {
    if (index >= _count)
        return nil;
    return _BUFFER_PTR_AT_INDEX(index);
}

void
CArray::sort() {
    if (!_isSupportComparable()) throw new ExComparator;
    if (_isSorted) return;
    uALGO::qSort(
        _buffer, _count, _itemSize,
        _comparatorCallback, (void*)this);

    _isSorted = true;
}

void*
CArray::firstObjectCommonWithArray(const CArray* otherArray) const {
    if (!otherArray) return nil;
    _CHECK_ITEMS_SIZE(otherArray);

    for (dInteger i = 0; i < _count; ++i) {
        void* object = _BUFFER_PTR_AT_INDEX(i);
        if (kNotFound != otherArray->indexOfObject(object))
            return object;
    }
    return nil;
}

bool
CArray::isEqualToArray(const CArray* otherArray) const {
    if (!otherArray) return false;
    _CHECK_ITEMS_SIZE(otherArray);
    if (otherArray->_count != _count) return false;

    for (dInteger i = 0; i < _count; ++i)
        if (kCompareEqualTo != _compare(
                objectAtIndex(i),
                otherArray->objectAtIndex(i)))
            return false;

    return true;
}

void
CArray::addObject(const void* object) {
    insertObjectAtIndex(object, _count);
}

void
CArray::addObjectsFromArray(const CArray* otherArray) {
    if (!otherArray) return;
    _CHECK_ITEMS_SIZE(otherArray);

    setCapacity(_count + otherArray->_count);
	_isSorted = false;
	
    _copyItems(
        _BUFFER_PTR_AT_INDEX(_count),
        otherArray->_buffer,
        otherArray->_count);
    _count += otherArray->_count;
}

void
CArray::insertObjectAtIndex(const void* object, dInteger index) {
	CL_THROW_IF_NIL(object);
    if (index > _count) throw new ExRange(index);

    setCapacity(_count + 1);
	_isSorted = false;
	
    if (index != _count)
        std::memmove(
            _BUFFER_PTR_AT_INDEX(index + 1),
            _BUFFER_PTR_AT_INDEX(index),
            (_count - index) * _itemSize);

    _copyItems(_BUFFER_PTR_AT_INDEX(index), object, 1);
    ++_count;
}

void
CArray::setObjectsToRange(const void* objects, dRange rng, bool deleteOutRangeTail) {
    if (rng.location > _count) throw new ExRange(rng.location);
    if (!objects) return;
	
    _isSorted = false;
	
    dInteger total = rng.location + rng.length;
    if (total > _count) {
        _destroyItems(_BUFFER_PTR_AT_INDEX(rng.location), _count - rng.location);
        setCapacity(total);
        _count += total - _count;
    } else if (deleteOutRangeTail) {
        _destroyItems(_BUFFER_PTR_AT_INDEX(rng.location), _count - rng.location);
        _count -= _count - total;
    } else {
        _destroyItems(_BUFFER_PTR_AT_INDEX(rng.location), rng.length);
    }
	
    _copyItems(
		_BUFFER_PTR_AT_INDEX(rng.location),
		objects,
		rng.length);
}

void
CArray::removeAllObjects() {
    try {
        removeObjectsInRange(dMakeRange(0, _count));
    } catch (ExRange* e) {
        // It's Ok!
        SAFE_DELETE(e);
    } catch (...) {
        throw;
    }
}

void
CArray::removeObject(const void* object) {
    removeObjectInRange(object, dMakeRange(0, _count));
}

void
CArray::removeObjectInRange(const void* object, dRange range) {
    if (0 == _count) return;
    dRangeCheck(range, _count);

    dInteger index = range.location + range.length;
    while (kNotFound != (index = _isSorted
        ? uALGO::binarySearch(
            _buffer, object,
            index - 1, false,
            _count, _itemSize,
            _comparatorCallback, (void*)this)
        : uALGO::linearSearch(
            _buffer, object,
            index - 1, false,
            _count, _itemSize,
            _comparatorCallback, (void*)this))
        && index >= range.location) {
            removeObjectAtIndex(index);
    }
}

void
CArray::removeLastObject() {
    removeObjectAtIndex(_count - 1);
}

void
CArray::removeObjectAtIndex(dInteger index) {
    if (index >= _count || index < 0) throw new ExRange(index);

    _destroyItems(_BUFFER_PTR_AT_INDEX(index), 1);
    if (index != _count - 1)
        std::memmove(
            _BUFFER_PTR_AT_INDEX(index),
            _BUFFER_PTR_AT_INDEX(index + 1),
            (_count - index - 1) * _itemSize);
    --_count;
    _CHECK_NOT_USED_MEMORY;
}

void
CArray::removeObjectsFromIndices(dInteger* indices, dInteger count) {
    if (0 == _count) return;

    dRange* rngs = new dRange[count];
    dInteger realCount = count;

    _searchSequentials(indices, rngs, &realCount);
    _calcIndices(rngs, realCount);
    for (dInteger i = 0; i < realCount; ++i)
        removeObjectsInRange(rngs[i]);

    SAFE_DELETE_ARRAY(rngs);
}

void
CArray::removeObjectsInArray(const CArray* otherArray) {
    _CHECK_ITEMS_SIZE(otherArray);
    for (dInteger i = 0; i < otherArray->_count; ++i)
        removeObject(otherArray->objectAtIndex(i));
}

void
CArray::removeObjectsInRange(dRange range) {
    dRangeCheck(range, _count);

    _destroyItems(_BUFFER_PTR_AT_INDEX(range.location), range.length);
    std::memmove(
        _BUFFER_PTR_AT_INDEX(range.location),
        _BUFFER_PTR_AT_INDEX(range.location + range.length),
        (_count - range.location - range.length) * _itemSize);
    _count -= range.length;
    _CHECK_NOT_USED_MEMORY;
}

void
CArray::replaceObjectAtIndexWithObject(dInteger index, const void* object) {
    if (index >= _count) throw new ExRange(index);
	CL_THROW_IF_NIL(object);
    if (_BUFFER_PTR_AT_INDEX(index) == object) return;

	_isSorted = false;
    _destroyItems(_BUFFER_PTR_AT_INDEX(index), 1);
    _copyItems(_BUFFER_PTR_AT_INDEX(index), object, 1);
}

void
CArray::replaceObjectsInRangeWithObjectsFromArrayRange(
    dRange range,
    const CArray* otherArray,
    dRange otherRange) {

    dRangeCheck(range, _count);
    _CHECK_ITEMS_SIZE(otherArray);
    dRangeCheck(otherRange, otherArray->_count);
    
	_isSorted = false;
	
    if (range.length == otherRange.length) {
        _destroyItems(_BUFFER_PTR_AT_INDEX(range.location), range.length);
        _copyItems(
            _BUFFER_PTR_AT_INDEX(range.location),
            otherArray->_buffer + otherRange.location * _itemSize,
            range.length);
    } else if (range.length > otherRange.length) {
        _destroyItems(_BUFFER_PTR_AT_INDEX(range.location), otherRange.length);
        _copyItems(
            _BUFFER_PTR_AT_INDEX(range.location),
            otherArray->_buffer + otherRange.location * _itemSize,
            otherRange.length);
        removeObjectsInRange(dMakeRange(
            range.location + otherRange.length - 1,
            range.length - otherRange.length));
    } else {
        setCapacity(_count + otherRange.length - range.length);
        std::memmove(
            _BUFFER_PTR_AT_INDEX(range.location + otherRange.length),
            _BUFFER_PTR_AT_INDEX(range.location + range.length),
            (_count - range.location - range.length) * _itemSize);

        _destroyItems(_BUFFER_PTR_AT_INDEX(range.location), range.length);
        _copyItems(
            _BUFFER_PTR_AT_INDEX(range.location),
            otherArray->_buffer + otherRange.location * _itemSize,
            otherRange.length);
        _count += otherRange.length - range.length;
    }
}

void
CArray::replaceObjectsInRangeWithObjectsFromArray(dRange range, const CArray* otherArray) {
    replaceObjectsInRangeWithObjectsFromArrayRange(
        range,
        otherArray,
       dMakeRange(0, otherArray->_count));
}

void
CArray::setArray(CArray* otherArray) {
    if (!otherArray) return;
    otherArray->replaceObjectsInRangeWithObjectsFromArray(
       dMakeRange(0, otherArray->_count),
        this);
}

void
CArray::setCapacity(dInteger newCapacity, dInteger growBy) {
	CL_THROW_IF_TRUE(newCapacity < 0);
    if (growBy >= 0) _growBy = growBy;

    if (0 == newCapacity) {
        if (_buffer) {
            _destroyItems(_buffer, _count);
            SAFE_DELETE_ARRAY(_buffer);
        }
        _count = _totalBlocks = 0;
    } else if (!_buffer) {
        _totalBlocks = __max(newCapacity, _growBy);
        _buffer = new dByte[_totalBlocks * _itemSize];
    } else if (newCapacity <= _totalBlocks) {
        if (_count > newCapacity) {
            _destroyItems(
                _BUFFER_PTR_AT_INDEX(newCapacity),
                _count - newCapacity);
            _count = newCapacity;
        }
    } else {
        growBy = _growBy;
        if (0 == growBy) {
            growBy = _count / 8;
            growBy = (growBy < 4) ? 4 : ((growBy > 1024) ? 1024 : growBy);
        }
        dInteger newTotalBlocks = (newCapacity < _totalBlocks + growBy)
            ? _totalBlocks + growBy
            : newCapacity;

		CL_THROW_IF_TRUE(newTotalBlocks < _totalBlocks);

        dByte* newBuffer = new dByte[newTotalBlocks * _itemSize];
        std::memcpy(newBuffer, _buffer, _count * _itemSize);

        SAFE_DELETE_ARRAY(_buffer);
        _buffer = newBuffer;
        _totalBlocks = newTotalBlocks;
    }
}

void
CArray::_freeNotUsedMemory() {
    if (_count == _totalBlocks) return;

    dByte* newBuffer = nil;
    if (0 != _count) {
        newBuffer = new dByte[_count * _itemSize];
        std::memcpy(newBuffer, _buffer, _count * _itemSize);
    }

    SAFE_DELETE_ARRAY(_buffer);
    _buffer = newBuffer;
    _totalBlocks = _count;
}

void
CArray::_copyItems(void* dst, const void* src, dInteger count) const {
    std::memcpy(dst, src, count * _itemSize);
}

void
CArray::_destroyItems(void* objPtr, dInteger count) const {
    // must be overriden in derived template class
}

dComparisonResult
CArray::_compare(const void* obj1, const void* obj2) const {
    return uALGO::defaultComparator(
        *(dUInteger*)_comparatorContext(),
        obj1,
        obj2);
}

void*
CArray::_comparatorContext() const {
    return (void*)&_itemSize;
}

bool
CArray::_isSupportComparable() const {
    return false;
}

dComparisonResult
CArray::_comparatorCallback(const void* obj1, const void* obj2, void* context) {
    return ((CArray*)context)->_compare(obj1, obj2);
}

static void
_searchSequentials(dInteger* inNumArray, dRange* outSeqArray, dInteger* inOutCnt) {
    dInteger cnt = 0;
    for (dInteger i = 0; i < *inOutCnt; ++i) {
        if (i > 1 && inNumArray[i] == outSeqArray[cnt - 1].location) continue;
        outSeqArray[cnt].location = inNumArray[i];
        for (
            outSeqArray[cnt].length = 1;
            outSeqArray[cnt].length < *inOutCnt && (inNumArray[i] == inNumArray[i + 1] - 1);
            ++outSeqArray[cnt].length, ++i); // empty loop
        ++cnt;
    }
    *inOutCnt = cnt;
}

static void
_calcIndices(dRange* inOutSeqArray, dInteger cnt) {
    for (dInteger i = 0; i < cnt; ++i) {
        for (dInteger j = i + 1; j < cnt; ++j)
            if (inOutSeqArray[j].location > inOutSeqArray[i].location)
                inOutSeqArray[j].location -= inOutSeqArray[i].length;
    }
}