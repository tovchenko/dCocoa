/*
** $Id: 2009/02/07, Taras Tovchenko $
** Private class which need use for realization real template-array class
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_CARRAY_H
#define _COCOA_CARRAY_H



BEGIN_NAMESPACE_CL

class DLL_PUBLIC CArray {
public:
    CArray();
    // initializing an array
    void                        initWithArray(const CArray* otherArray);
    void                        initWithObjects(dUInteger itemSize, ...); // must end with nil
    void                        initWithCArray(dUInteger itemSize, const void* objects, dInteger count);
    // querying an array
    bool                        containsObject(const void* object) const;
    dInteger                    count() const;
    void                        getObjects(void* buffer) const;
    void                        getObjectsInRange(void* buffer, dRange range) const;
    dInteger                    indexOfObject(const void* object) const;
    dInteger                    indexOfObjectInRange(const void* object, dRange range) const;
    void*                       lastObject() const;
    void*                       objectAtIndex(dInteger index) const;
	// works only for pointer types
	void*						objectAtIndexNoException(dInteger index) const;
    // comparing & sorting
    // if _isSupportComparable == 'true' than supports following operations >, <, ==, !=
    // otherwise supports only ==, !=
    void                        sort();
    void*                       firstObjectCommonWithArray(const CArray* otherArray) const;
    bool                        isEqualToArray(const CArray* otherArray) const;
    // adding objects
    void                        addObject(const void* object);
    void                        addObjectsFromArray(const CArray* otherArray);
    void                        insertObjectAtIndex(const void* object, dInteger index);
	void                        setObjectsToRange(const void* objects, dRange rng, bool deleteOutRangeTail); // you must put count of objects in rng.length
    // removing objects
    void                        removeAllObjects();
    void                        removeLastObject();
    void                        removeObject(const void* object);
    void                        removeObjectInRange(const void* object, dRange range);
    void                        removeObjectAtIndex(dInteger index);
    void                        removeObjectsFromIndices(dInteger* indices, dInteger count);
    void                        removeObjectsInArray(const CArray* otherArray);
    void                        removeObjectsInRange(dRange range);
    // replacing objects
    void                        replaceObjectAtIndexWithObject(dInteger index, const void* object);
    void                        replaceObjectsInRangeWithObjectsFromArrayRange(dRange range, const CArray* otherArray, dRange otherRange);
    void                        replaceObjectsInRangeWithObjectsFromArray(dRange range, const CArray* otherArray);
    void                        setArray(CArray* otherArray);
    // 'newCapacity' - how many items need you now, 'growBy' - on how many additional items
    // you want allocate when array resize, (-1) - stay previous, (0) - auto
    void                        setCapacity(dInteger newCapacity, dInteger growBy = -1);

protected:
    // forbidden dtor
    virtual ~CArray();
    // _dealloc free used memory, in template sub-class
    // you must call dtor for each item in this array
    void                        _dealloc();
    void                        _freeNotUsedMemory();
    // this method must be override in template sub-class,
    // need call copy-ctor for each copying object
    virtual void                _copyItems(void* dst, const void* src, dInteger count) const;
    virtual void                _destroyItems(void* objPtr, dInteger count) const;

    virtual dComparisonResult   _compare(const void* obj1, const void* obj2) const;
    virtual void*               _comparatorContext() const;
    virtual bool                _isSupportComparable() const;


    dUInteger                   _itemSize;
    dInteger                    _count;
    dByte*                      _buffer;
    dInteger                    _totalBlocks;
    dInteger                    _growBy;

    bool                        _isSorted;

private:
    // forbidden
    CArray(const CArray& rf);
    CArray& operator=(const CArray& rf);
    // use for passed to functions witch want UComparatorCallback
    static dComparisonResult    _comparatorCallback(const void* obj1, const void* obj2, void* context);
};

END_NAMESPACE_CL

#endif // _COCOA_CARRAY_H