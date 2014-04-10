/*
** $Id: 2009/02/07, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "CHashTable.h"
#include "CAlgorithms.h"

#include <memory>

#define _BUCKET_INDEX(_obj_)            ((dUInteger)_hash(_obj_) % _buckets->count())
#define _CHECK_ITEMS_SIZE(_tbl_)        CL_THROW_IF_TRUE(_itemSize != _tbl_->_itemSize)

static const dInteger  kDefaultCapacity = 16;


CHashTable::CHashTable()
: _itemSize(0),
  _count(0),
  _buckets(nil) {
}

CHashTable::~CHashTable() {
}

void
CHashTable::initWithDefaults(dUInteger itemSize) {
    initWithCapacity(itemSize, kDefaultCapacity);
}

void
CHashTable::initWithCapacity(dUInteger itemSize, dInteger capacity) {
    _itemSize = itemSize;
    _buckets = new CArrayT<_HashBucket*, _HashBucket*>;
    _addEmptyBucketsForCapacity(capacity);
}

void
CHashTable::initWithCArray(dUInteger itemSize, const void* objects, dInteger count) {
    initWithCapacity(itemSize, count);

    while (count--) {
        addObject(objects);
        objects = ((dByte*)objects) + _itemSize;
    }
}

void
CHashTable::initWithHashTable(const CHashTable* otherTable) {
	CL_THROW_IF_NIL(otherTable);

    initWithCapacity(otherTable->_itemSize, otherTable->_buckets->count());

    CHashTable::Iterator iter(otherTable);
    while (iter.next()) addObject(iter.object());
}

void
CHashTable::_dealloc() {
    removeAllObjects();
    SAFE_DELETE(_buckets);
}

dInteger
CHashTable::count() const {
    return _count;
}

bool
CHashTable::isEqualToHashTable(const CHashTable* otherTable) {
	CL_THROW_IF_NIL(otherTable);
    _CHECK_ITEMS_SIZE(otherTable);

    if (_count != otherTable->_count) return false;

    CHashTable::Iterator iter(this);
    while (iter.next()) {
        void* otherObj = otherTable->objectForKey(iter.object());
        if (!otherObj) return false;
        if (kCompareEqualTo != _compare(iter.object(), otherObj)) return false;
    }
    return true;
}

void*
CHashTable::objectForKey(const void* objectKey) const {
	CL_THROW_IF_NIL(objectKey);
    _HashBucket* bucket = _buckets->objectAtIndex(_BUCKET_INDEX(objectKey));
    if (NIL_OBJ == bucket) return nil;

    for (dInteger i = 0, cnt = bucket->count(); i < cnt; ++i) {
        void* iobj = bucket->objectAtIndex(i);
        if (kCompareEqualTo == _isEqualKeys(objectKey, iobj))
            return iobj;
    }
    return nil;
}

void
CHashTable::addObject(const void* object) {
	CL_THROW_IF_NIL(object);
    dInteger hashIndex = _BUCKET_INDEX(object);
    _HashBucket* bucket = _buckets->objectAtIndex(hashIndex);

    if (NIL_OBJ == bucket) {
        bucket = new _HashBucket;
        bucket->addObject(_allocCopy(object));
        _buckets->replaceObjectAtIndexWithObject(hashIndex, bucket);
    } else {
        for (dInteger i = 0, cnt = bucket->count(); i < cnt; ++i) {
            void* iobj = bucket->objectAtIndex(i);
            if (kCompareEqualTo == _isEqualKeys(object, iobj)) {
                _allocDestroy(iobj);
                bucket->replaceObjectAtIndexWithObject(i, _allocCopy(object));
                return;
            }
        }

        bucket->addObject(_allocCopy(object));
    }
    if (++_count > _buckets->count())
        _rehashToCapacity(_buckets->count() * 2 + 1);
}

void
CHashTable::addObjectsFromHashTable(const CHashTable* table) {
    if (!table) return;
    _CHECK_ITEMS_SIZE(table);

    Iterator iter(table);
    while (iter.next()) addObject(iter.object());
}

void
CHashTable::removeObject(const void* object) {
	CL_THROW_IF_NIL(object);
    _HashBucket* bucket = _buckets->objectAtIndex(_BUCKET_INDEX(object));

    if (NIL_OBJ == bucket) return;

    for (dInteger i = 0, cnt = bucket->count(); i < cnt; ++i) {
        void* iobj = bucket->objectAtIndex(i);
        if (kCompareEqualTo == _isEqualKeys(object, iobj)) {
            _allocDestroy(iobj);
            bucket->removeObjectAtIndex(i);
            --_count;
            return;
        }
    }
}

void
CHashTable::removeAllObjects() {
    _removeAllObjects(_buckets);
    _count = 0;
}

void
CHashTable::_copyItem(void* dst, const void* src) const {
    std::memcpy(dst, src, _itemSize);
}

void
CHashTable::_destroyItem(void* item) const {
}

dComparisonResult
CHashTable::_compare(const void* obj1, const void* obj2) const {
    return uALGO::defaultComparator(
        _itemSize,
        obj1,
        obj2);
}

void*
CHashTable::_comparatorContext() const {
    return nil;
}

CHashTable::Iterator::Iterator(const CHashTable* hashTable) {
    _hashTable = hashTable;
    reset();
}

CHashTable::Iterator::Iterator()
: _hashTable(nil) {
}

void
CHashTable::Iterator::reset() {
    if (!_hashTable || 0 == _hashTable->_buckets->count())
        return;

    _i = _hashTable->_buckets->count();
    _j = 0;
    _current = nil;
}

bool
CHashTable::Iterator::next() {
    return _next(_hashTable->_buckets);
}

bool
CHashTable::Iterator::_next(CArrayT<_HashBucket*, _HashBucket*>* buckets) {
    if (!buckets || 0 == buckets->count())
        return false;
    
    while (0 == _j) {
        if (0 == _i) return false;
        --_i;
        
        _HashBucket* bucket;
        _j = (buckets)
        ? (NIL_OBJ != (bucket = buckets->objectAtIndex(_i))) ? bucket->count() : 0
        : 0;
    }
    
    --_j;
    _current = buckets->objectAtIndex(_i)->objectAtIndex(_j);
    return true;
}

void*
CHashTable::Iterator::object() const {
    return _current;
}

void
CHashTable::_rehashToCapacity(dInteger newCapacity) {
    dUInteger itemSize = _itemSize;
    std::auto_ptr<CArrayT<_HashBucket*, _HashBucket*> > localBuckets(_buckets);
    try {
        _buckets = nil;
        _count = 0;
        _dealloc();
        initWithCapacity(itemSize, newCapacity);

        CHashTable::Iterator it;
        it._i = localBuckets->count();
        it._j = 0;
        it._current = nil;
        while (it._next(localBuckets.get())) addObject(it.object());
    } catch (...) {
        _removeAllObjects(localBuckets.get());
        throw;
    }
    _removeAllObjects(localBuckets.get());
}

void
CHashTable::_addEmptyBucketsForCapacity(dInteger capacity) {
	CL_THROW_IF_TRUE(capacity < _buckets->count());
    _buckets->setCapacity(capacity);
    capacity -= _buckets->count();

    while (capacity--) _buckets->addObject((_HashBucket*)NIL_OBJ);
}

void
CHashTable::_removeAllObjects(CArrayT<_HashBucket*, _HashBucket*>* buckets) {
    if (!buckets) return;

    for (dInteger i = 0, cnt = buckets->count(); i < cnt; ++i) {
        _HashBucket* bucket = buckets->objectAtIndex(i);
        if (NIL_OBJ != bucket) {
            for (dInteger j = 0, jcnt = bucket->count(); j < jcnt; ++j)
                _allocDestroy(bucket->objectAtIndex(j));

            SAFE_DELETE(bucket);
            buckets->replaceObjectAtIndexWithObject(i, (_HashBucket*)NIL_OBJ);
        }
    }
}

void*
CHashTable::_allocCopy(const void* src) const {
    void* dst = new dByte[_itemSize];
    _copyItem(dst, src);
    return dst;
}

void
CHashTable::_allocDestroy(void* object) const {
    _destroyItem(object);
	delete[] (dByte*)object;
}