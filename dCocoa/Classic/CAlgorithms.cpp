/*
** $Id: 2009/02/07, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "CAlgorithms.h"

#include <limits>
#include <memory>

#define _COMPARE(_obj1_, _obj2_)  ((*comparatorCallback)(_obj1_, _obj2_, comparatorContext))

static const dInteger kUIntegerSize = sizeof(dInteger);
static const dInteger kMaxStack = sizeof(dUInteger) * CHAR_BIT;

static void      _swap(void* obj1, void* obj2, dUInteger itemSize);



void
uALGO::qSort(
    void* base,
    dInteger itemsCount,
    dUInteger itemSize,
    dComparatorFunction comparatorCallback,
    void* comparatorContext)
{
	CL_THROW_IF_TRUE(!base || !comparatorCallback);

    void* leftStack[kMaxStack];
    void* rightStack[kMaxStack];

    leftStack[0] = base;
    rightStack[0] = (dByte*)base + (itemsCount - 1) * itemSize;

    for (dInteger sp = 0; sp >= 0; --sp) {
        dByte* left = (dByte*)leftStack[sp];
        dByte* right = (dByte*)rightStack[sp];

        while (left < right) {
            unsigned long offset = (right - left) / 2;
            dByte* pivotal = left + offset - offset % itemSize;
            _swap(left, pivotal, itemSize);

            dByte* i = left + itemSize;
            dByte* j = right;
            while (true) {
                while (i < j && kCompareGreaterThan == _COMPARE(left, i)) i += itemSize;
                while (j >= i && kCompareLessThan == _COMPARE(left, j)) j -= itemSize;
                if (i >= j) break;
                _swap(i, j, itemSize);
                j -= itemSize;
                i += itemSize;
            }
            _swap(left, j, itemSize);

            dByte* middle = j;
            if (middle - left <= right - middle) {
                if (middle + itemSize < right) {
                    leftStack[sp] = middle + itemSize;
                    rightStack[sp++] = right;
                }
                right = middle - itemSize;
            } else {
                if (middle - itemSize > left) {
                    leftStack[sp] = left;
                    rightStack[sp++] = middle - itemSize;
                }
                left = middle + itemSize;
            }
        }
    }
}

dInteger
uALGO::linearSearch(
    const void* base,
    const void* object,
    dInteger startPosition,
    bool direction,
    dInteger itemsCount,
    dUInteger itemSize,
    dComparatorFunction comparatorCallback,
    void* comparatorContext)
{
	CL_THROW_IF_TRUE(!base || startPosition > itemsCount || !comparatorCallback);

    dByte*  ptr = (dByte*)base + startPosition * itemSize;
    if (direction) {
        for (dInteger i = startPosition; i < itemsCount; ++i, ptr += itemSize)
            if (kCompareEqualTo == _COMPARE(ptr, object))
                return i;
    } else {
        for (dInteger i = startPosition; i >= 0; --i, ptr -= itemSize)
            if (kCompareEqualTo == _COMPARE(ptr, object))
                return i;
    }
    return kNotFound;
}

dInteger
uALGO::binarySearch(
    const void* base,
    const void* object,
    dInteger startPosition,
    bool direction,
    dInteger itemsCount,
    dUInteger itemSize,
    dComparatorFunction comparatorCallback,
    void* comparatorContext)
{
	CL_THROW_IF_TRUE(!base || startPosition > itemsCount || !comparatorCallback);

    dInteger left = direction ? startPosition : 0;
    dInteger right = direction ? itemsCount - 1 : startPosition;
    while (left <= right) {
        dInteger current = (left + right) / 2;
        dComparisonResult result = _COMPARE((dByte*)base + current * itemSize, object);

        if (kCompareEqualTo == result) return current;
        else if (kCompareGreaterThan == result) right = current - 1;
        else if (kCompareLessThan == result) left = current + 1;
    }
    return kNotFound;
}

dComparisonResult
uALGO::defaultComparator(
    dUInteger itemSize,
    const void* obj1,
    const void* obj2) {

    if (!std::memcmp(obj1, obj2, itemSize))
        return kCompareEqualTo;
    return !kCompareEqualTo;
}

static void
_swap(void* obj1, void* obj2, dUInteger itemSize) {
    dUInteger i;

    for (i = kUIntegerSize; i <= itemSize; i += kUIntegerSize) {
        dInteger tmp = *(dInteger*)obj1;
        *(dInteger*)obj1 = *(dInteger*)obj2;
        obj1 = (dInteger*)obj1 + 1;
        *(dInteger*)obj2 = tmp;
        obj2 = (dInteger*)obj2 + 1;
    }
    for (i = i - kUIntegerSize + 1; i <= itemSize; ++i) {
        dByte tmp = *(dByte*)obj1;
        *(dByte*)obj1 = *(dByte*)obj2;
        obj1 = (dByte*)obj1 + 1;
        *(dByte*)obj2 = tmp;
        obj2 = (dByte*)obj2 + 1;
    }
}

dUInteger
uALGO::hashFromString(const char* str) {
    dUInteger hash = 0;
    for (;;) {
        dUInteger a = *str++;
        if (0 == a) break;
        hash += (hash << 8) + a;
    }

    return hash;
}

dUInteger
uALGO::hashFromByteSequence(const dByte* buffer, dUInteger size) {
    dUInteger hash = 0;
    for (;0 != size; --size) {
        dUInteger a = *buffer++;
        hash += (hash << 8) + a;
    }

    return hash;
}