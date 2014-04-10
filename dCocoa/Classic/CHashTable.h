/*
** $Id: 2009/02/07, Taras Tovchenko $
** Private class which need use for realization real templeate class (Dictionary and Set)
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_CHASHTABLE_H
#define _COCOA_CHASHTABLE_H

#include "CArrayT.h"


BEGIN_NAMESPACE_CL


typedef CArrayT<void*, void*>    _HashBucket;

class DLL_PUBLIC CHashTable {
public:
    CHashTable();
    void                        initWithDefaults(dUInteger itemSize);
    void                        initWithCapacity(dUInteger itemSize, dInteger capacity);
    void                        initWithCArray(dUInteger itemSize, const void* objects, dInteger count);
    void                        initWithHashTable(const CHashTable* otherTable);
    // attributes
    dInteger                    count() const;
    // comparing hash tables
    bool                        isEqualToHashTable(const CHashTable* otherTable);
    // accessing
    void*                       objectForKey(const void* objectKey) const;
    // adding
    void                        addObject(const void* object);
    void                        addObjectsFromHashTable(const CHashTable* table);
    // removing
    void                        removeObject(const void* object);
    void                        removeAllObjects();

protected:
    // forbidden dtor
    virtual ~CHashTable();
    virtual void                _dealloc();
    // this methods must be override in template sub-class,
    // need call copy-ctor for each copying object
    virtual void                _copyItem(void* dst, const void* src) const;
    virtual void                _destroyItem(void* item) const;
    // hashing
    virtual dInteger            _hash(const void* object) const = 0;
    virtual dComparisonResult   _isEqualKeys(const void* obj1, const void* obj2) const = 0;
    // comparing
    virtual dComparisonResult   _compare(const void* obj1, const void* obj2) const;
    virtual void*               _comparatorContext() const;


    dUInteger                               _itemSize;
    dInteger                                _count;
    CArrayT<_HashBucket*, _HashBucket*>*    _buckets;

    // iterating
    class DLL_PUBLIC Iterator {
    friend class CHashTable;
    public:
        Iterator(const CHashTable* hashTable);
        // operations
        void            reset();
        bool            next();
        void*           object() const;

    protected:
        Iterator();
        bool                _next(CArrayT<_HashBucket*, _HashBucket*>* buckets);
        
        const CHashTable*   _hashTable;
        dInteger            _i, _j;
        void*               _current;
    };

private:
    void                        _rehashToCapacity(dInteger capacity);
    void                        _addEmptyBucketsForCapacity(dInteger capacity);
    void                        _removeAllObjects(CArrayT<_HashBucket*, _HashBucket*>* buckets);
    void*                       _allocCopy(const void* src) const;
    void                        _allocDestroy(void* object) const;
    // forbidden
    CHashTable(const CHashTable& rf);
    CHashTable& operator=(const CHashTable& rf);
};

END_NAMESPACE_CL

#endif // _COCOA_CHASHTABLE_H