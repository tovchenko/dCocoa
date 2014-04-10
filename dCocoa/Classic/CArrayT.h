/*
** $Id: 2009/02/07, Taras Tovchenko $
** Real template array class which use private realization of CArray
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_CARRAYT_H
#define _COCOA_CARRAYT_H

#include "CArray.h"

#include <cstdarg>
#include <new>


#define _DECLARE_ARRAY_TEMPLATE template <    \
                            typename TYPE,    \
                            typename ARG_TYPE,    \
                            template <typename> class COPY_DELETE_FUNCS>

#define _CLASS_ARRAY_TEMPLATE CArrayT<    \
                            TYPE,    \
                            ARG_TYPE,    \
                            COPY_DELETE_FUNCS>


BEGIN_NAMESPACE_CL

template <typename TYPE>
class CCopyDeleteClassicArrayPolicyT {
public:
    static void     copyItems(TYPE* dst, const TYPE* src, dInteger count) {
        while (count--) {
            new (dst) TYPE(*src);
            dst = dst + 1;
            src = src + 1;
        }
    }

    static void     destroyItems(TYPE* objPtr, dInteger count) {
        if (!objPtr) return;
        for (dInteger i = 0; i < count; ++i)
            (objPtr + i)->~TYPE();
    }
};


template <
    typename TYPE,
    typename ARG_TYPE = TYPE&,
    template <typename> class COPY_DELETE_FUNCS = CCopyDeleteClassicArrayPolicyT>
class CArrayT : private CArray {
public:
    // this ctor is not efficient because call copy-ctors for objects witch passeds through elipsis
//  CArrayT(TYPE first, ...); // must end of NIL_OBJ, this method dont work on mac 64-bit
    CArrayT() { _itemSize = sizeof(TYPE); }
    CArrayT(const TYPE* objects, dInteger count) { CArray::initWithCArray(sizeof(TYPE), objects, count); }
    CArrayT(const _CLASS_ARRAY_TEMPLATE* otherArray) { CArray::initWithArray(otherArray); }
    CArrayT(const _CLASS_ARRAY_TEMPLATE& otherArray) { CArray::initWithArray(&otherArray); }
    virtual ~CArrayT() { CArray::_dealloc(); }
    // querying an array
    bool            containsObject(ARG_TYPE object) const { return CArray::containsObject(&object); }
    dInteger        count() const { return CArray::count(); }
    void            getObjects(TYPE* buffer) const { CArray::getObjects(buffer); }
    void            getObjectsInRange(TYPE* buffer, dRange range) const { CArray::getObjectsInRange(buffer, range); }
    dInteger        indexOfObject(ARG_TYPE object) const { return CArray::indexOfObject(&object); }
    dInteger        indexOfObjectInRange(ARG_TYPE object, dRange range) const { return CArray::indexOfObjectInRange(&object, range); }
    ARG_TYPE        lastObject() const { return *(TYPE*)CArray::lastObject(); }
    ARG_TYPE        objectAtIndex(dInteger index) const { return *(TYPE*)CArray::objectAtIndex(index); }
	// works only for pointer types
	ARG_TYPE		objectAtIndexNoException(dInteger index) const { void* result = CArray::objectAtIndexNoException(index); return (result) ? *(TYPE*)result : nil; }
    // comparing & sorting
    // if isComparable == 'true' than supports following operations >, <, ==, !=
    // over wise supports only ==, !=
    void            sort() { CArray::sort(); }
    ARG_TYPE        firstObjectCommonWithArray(const _CLASS_ARRAY_TEMPLATE* otherArray) const { return *(TYPE*)CArray::firstObjectCommonWithArray(otherArray); }
    bool            isEqualToArray(const _CLASS_ARRAY_TEMPLATE* otherArray) const { return CArray::isEqualToArray(otherArray); }
    // adding objects
    void            addObject(TYPE object) { CArray::addObject(&object); }
    void            addObjectsFromArray(const _CLASS_ARRAY_TEMPLATE* otherArray) { CArray::addObjectsFromArray(otherArray); }
    void            insertObjectAtIndex(ARG_TYPE object, dInteger index) { CArray::insertObjectAtIndex(&object, index); }
	void            setObjectsToRange(const TYPE* objects, dRange rng, bool deleteOutRangeTail = false) { CArray::setObjectsToRange(objects, rng, deleteOutRangeTail); }
    // removing objects
    void            removeAllObjects() { CArray::removeAllObjects(); }
    void            removeLastObject() { CArray::removeLastObject(); }
    void            removeObject(ARG_TYPE object) { CArray::removeObject(&object); }
    void            removeObjectInRange(ARG_TYPE object, dRange range) { CArray::removeObjectInRange(&object, range); }
    void            removeObjectAtIndex(dInteger index) { CArray::removeObjectAtIndex(index); }
    void            removeObjectsFromIndices(dInteger* indices, dInteger count) { CArray::removeObjectsFromIndices(indices, count); }
    void            removeObjectsInArray(const _CLASS_ARRAY_TEMPLATE* otherArray) { CArray::removeObjectsInArray(otherArray); }
    void            removeObjectsInRange(dRange range) { CArray::removeObjectsInRange(range); }
    // replacing objects
    void            replaceObjectAtIndexWithObject(dInteger index, ARG_TYPE object) { CArray::replaceObjectAtIndexWithObject(index, &object); }
    void            replaceObjectsInRangeWithObjectsFromArrayRange(dRange range, const _CLASS_ARRAY_TEMPLATE* otherArray, dRange otherRange) { CArray::replaceObjectsInRangeWithObjectsFromArrayRange(range, otherArray, otherRange); }
    void            replaceObjectsInRangeWithObjectsFromArray(dRange range, const _CLASS_ARRAY_TEMPLATE* otherArray) { CArray::replaceObjectsInRangeWithObjectsFromArray(range, otherArray); }
    void            setArray(_CLASS_ARRAY_TEMPLATE* otherArray) { CArray::setArray(otherArray); }
    // 'newCapacity' - how many items need you now, 'growBy' - on how many additional items
    // you want allocate when array resize, (-1) - stay previous, (0) - auto
    void            setCapacity(dInteger newCapacity, dInteger growBy = -1) { CArray::setCapacity(newCapacity, growBy); }
    // deriving new arrays, this methods return allocated array
    // you must deallocate it when finish work with it
    _CLASS_ARRAY_TEMPLATE*  newArrayByAddingObject(ARG_TYPE object) const;
    _CLASS_ARRAY_TEMPLATE*  newArrayByAddingObjectsFromArray(const _CLASS_ARRAY_TEMPLATE* otherArray) const;
    _CLASS_ARRAY_TEMPLATE*  newSubarrayWithRange(dRange range) const;

protected:
    virtual void    _copyItems(void* dst, const void* src, dInteger count) const;
    virtual void    _destroyItems(void* objPtr, dInteger count) const;

private:
    // forbidden
    _CLASS_ARRAY_TEMPLATE& operator=(const _CLASS_ARRAY_TEMPLATE& rf);
};

/*
_DECLARE_ARRAY_TEMPLATE
_CLASS_ARRAY_TEMPLATE::CArrayT(TYPE first, ...) {
    CArray::initWithObjects(sizeof(TYPE), &first, nil);

    va_list argList;
    va_start(argList, first);

    while (true) {
#if defined(__GNUC__)
		va_list tmpList;
        va_copy(tmpList, argList);
#else
		va_list tmpList = argList;
#endif
        if (NIL_OBJ == va_arg(tmpList, void*)) break;
		
		TYPE object = va_arg(argList, TYPE);
        CArray::addObject(&object);
    }
    va_end(argList);
}
*/

_DECLARE_ARRAY_TEMPLATE
_CLASS_ARRAY_TEMPLATE*
_CLASS_ARRAY_TEMPLATE::newArrayByAddingObject(ARG_TYPE object) const {
    _CLASS_ARRAY_TEMPLATE* result = new _CLASS_ARRAY_TEMPLATE(this);

    result->addObject(object);
    return result;
}

_DECLARE_ARRAY_TEMPLATE
_CLASS_ARRAY_TEMPLATE*
_CLASS_ARRAY_TEMPLATE::newArrayByAddingObjectsFromArray(const _CLASS_ARRAY_TEMPLATE* otherArray) const {
    _CLASS_ARRAY_TEMPLATE* result = new _CLASS_ARRAY_TEMPLATE(this);

    result->addObjectsFromArray(otherArray);
    return result;
}

_DECLARE_ARRAY_TEMPLATE
_CLASS_ARRAY_TEMPLATE*
_CLASS_ARRAY_TEMPLATE::newSubarrayWithRange(dRange range) const {
    dRangeCheck(range, _count);

    ARG_TYPE firstObjectInRange = objectAtIndex(range.location);
    _CLASS_ARRAY_TEMPLATE* result = new _CLASS_ARRAY_TEMPLATE(&firstObjectInRange, range.length);

    result->_isSorted = _isSorted;
    return result;
}

_DECLARE_ARRAY_TEMPLATE
void
_CLASS_ARRAY_TEMPLATE::_copyItems(void* dst, const void* src, dInteger count) const {
    COPY_DELETE_FUNCS<TYPE>::copyItems((TYPE*)dst, (const TYPE*)src, count);
}

_DECLARE_ARRAY_TEMPLATE
void
_CLASS_ARRAY_TEMPLATE::_destroyItems(void* objPtr, dInteger count) const {
    COPY_DELETE_FUNCS<TYPE>::destroyItems((TYPE*)objPtr, count);
}

END_NAMESPACE_CL

#endif // _COCOA_CARRAYT_H