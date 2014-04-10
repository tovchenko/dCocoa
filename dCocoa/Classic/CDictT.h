/*
** $Id: 2009/02/07, Taras Tovchenko $
** Real template dictionary class which use private realization of CHashTable
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_CDICT_H
#define _COCOA_CDICT_H

#include "CHashTable.h"

#include <memory>


#define _DECLARE_DICT_TEMPLATE template <    \
                        typename OBJ_TYPE,    \
                        typename KEY_TYPE,    \
                        template <typename, typename> class HASH_EQUAL_FUNCS,    \
                        typename ARG_OBJ_TYPE,    \
                        typename ARG_KEY_TYPE,    \
                        template <typename, typename, typename> class COPY_DELETE_FUNCS>

#define _CLASS_DICT_TEMPLATE CDictT<    \
                        OBJ_TYPE, KEY_TYPE,    \
                        HASH_EQUAL_FUNCS,    \
                        ARG_OBJ_TYPE, ARG_KEY_TYPE,    \
                        COPY_DELETE_FUNCS>



BEGIN_NAMESPACE_CL

// helpers
template <typename T>
struct is_ptr {
    typedef T type_class;
    typedef T* type_ptr;
    enum VP { value_ = 0 };
};

template <typename T>
struct is_ptr<T*> {
    typedef T type_class;
    typedef T* type_ptr;
    enum VP { value_ = 1 };
};



template <typename OBJ_TYPE, typename KEY_TYPE, typename PAIR_TYPE>
class CCopyDeleteClassicDictPolicyT {
public:
    static void     copyItem(PAIR_TYPE* dst, const PAIR_TYPE* src) {
        dst->object = new OBJ_TYPE(*src->object);
        dst->key = new KEY_TYPE(*src->key);
    }

    static void     destroyItem(PAIR_TYPE* item) {
        SAFE_DELETE(item->object);
        SAFE_DELETE(item->key);
    }
};


template <
    typename OBJ_TYPE, typename KEY_TYPE,
    template <typename, typename> class HASH_EQUAL_FUNCS,
    typename ARG_OBJ_TYPE = OBJ_TYPE&, typename ARG_KEY_TYPE = KEY_TYPE&,
    template <typename, typename, typename> class COPY_DELETE_FUNCS = CCopyDeleteClassicDictPolicyT>
class CDictT : protected CHashTable {
protected:
    // helpers
    struct SPair {
        SPair() : object((OBJ_TYPE*)NIL_OBJ), key((KEY_TYPE*)NIL_OBJ) { }
        SPair(OBJ_TYPE* aObject, KEY_TYPE* aKey) : object(aObject), key(aKey) { }
        SPair(KEY_TYPE* aKey) : key(aKey), object((OBJ_TYPE*)NIL_OBJ) { }

        OBJ_TYPE*    object;
        KEY_TYPE*    key;
    };

public:
    // exceptions
    class ExDictObjectNotFound : public CException {
    public:
        ExDictObjectNotFound(ARG_KEY_TYPE notFoundKey) {
            _key = std::auto_ptr<KEY_TYPE>(new KEY_TYPE(notFoundKey));
        }
        ARG_KEY_TYPE    notFoundKey() { return *_key.get(); }

    protected:
        std::auto_ptr<KEY_TYPE>    _key;
    };
    // iterating
    class Iterator {
    public:
        Iterator(_CLASS_DICT_TEMPLATE* dict);

        void                reset();
        bool                next();
        ARG_OBJ_TYPE        object() const;
        ARG_KEY_TYPE        key() const;

    protected:
        CHashTable::Iterator    _iter;
    };
    // interface
//  CDictT(OBJ_TYPE first, ...); // object0 - key0, ... , objectN - keyN, NIL_OBJ, this method dont work on mac 64-bit
    CDictT() { CHashTable::initWithDefaults(sizeof(SPair)); }
    CDictT(const OBJ_TYPE* cObjectsArray, const KEY_TYPE* cKeysArray, dInteger count);
    CDictT(dInteger capacity) { CHashTable::initWithCapacity(sizeof(SPair), capacity); }
    CDictT(const _CLASS_DICT_TEMPLATE* otherDict) { CHashTable::initWithHashTable(otherDict); }
    CDictT(const _CLASS_DICT_TEMPLATE& otherDict) { CHashTable::initWithHashTable(&otherDict); }
    virtual ~CDictT() { CHashTable::_dealloc(); }
    // counting entries
    dInteger                    count() const { return CHashTable::count(); }
    // comparing dictionaries
    bool                        isEqualToDictionary(const _CLASS_DICT_TEMPLATE* otherDict) { return CHashTable::isEqualToHashTable(otherDict); }
    // accessing keys and values
    void                        allKeys(CArrayT<KEY_TYPE, ARG_KEY_TYPE>* outKeys) const;
    void                        allKeysForObject(CArrayT<KEY_TYPE, ARG_KEY_TYPE>* outKeys, ARG_OBJ_TYPE anObject) const;
    void                        allValues(CArrayT<OBJ_TYPE, ARG_OBJ_TYPE>* outObjects) const;
    void                        getObjectsAndKeys(OBJ_TYPE* cObjectArray, KEY_TYPE* cKeyArray) const;
    ARG_OBJ_TYPE                objectForKey(ARG_KEY_TYPE key) const;
    // this method work only with pointer types
    ARG_OBJ_TYPE                objectForKeyNoException(ARG_KEY_TYPE key) const;
    void                        objectsForKeysNotFoundMarker(
                                        CArrayT<OBJ_TYPE, ARG_OBJ_TYPE>* outObjects,
                                        const CArrayT<KEY_TYPE, ARG_KEY_TYPE>* keys,
                                        ARG_OBJ_TYPE notFoundMarker) const;
    // adding entries
    void                        setObjectForKey(ARG_OBJ_TYPE object, ARG_KEY_TYPE key);
    void                        addEntriesFromDictionary(_CLASS_DICT_TEMPLATE* otherDictionary) { CHashTable::addObjectsFromHashTable(otherDictionary); }
    // removing entries
    void                        removeObjectForKey(ARG_KEY_TYPE key);
    void                        removeAllObjects() { CHashTable::removeAllObjects(); }
    void                        removeObjectsForKeys(CArrayT<KEY_TYPE, ARG_KEY_TYPE>* keyArray);

protected:
    virtual void                _copyItem(void* dst, const void* src) const;
    virtual void                _destroyItem(void* item) const;

    virtual dInteger            _hash(const void* object) const;
    virtual dComparisonResult   _isEqualKeys(const void* obj1, const void* obj2) const;

    virtual dComparisonResult   _compare(const void* obj1, const void* obj2) const;
};


_DECLARE_DICT_TEMPLATE
_CLASS_DICT_TEMPLATE::Iterator::Iterator(_CLASS_DICT_TEMPLATE* dict)
: _iter(dict) {

}

_DECLARE_DICT_TEMPLATE
inline void
_CLASS_DICT_TEMPLATE::Iterator::reset() {
    _iter.reset();
}

_DECLARE_DICT_TEMPLATE
inline bool
_CLASS_DICT_TEMPLATE::Iterator::next() {
    return _iter.next();
}

_DECLARE_DICT_TEMPLATE
ARG_OBJ_TYPE
_CLASS_DICT_TEMPLATE::Iterator::object() const {
    return *((SPair*)_iter.object())->object;
}

_DECLARE_DICT_TEMPLATE
ARG_KEY_TYPE
_CLASS_DICT_TEMPLATE::Iterator::key() const {
    return *((SPair*)_iter.object())->key;
}

/*
_DECLARE_DICT_TEMPLATE
_CLASS_DICT_TEMPLATE::CDictT(OBJ_TYPE first, ...) {
    CHashTable::initWithDefaults(sizeof(SPair));
	
    va_list argList;
    va_start(argList, first);
	
    KEY_TYPE firstKey = va_arg(argList, KEY_TYPE);
    SPair firstPair(&first, &firstKey);
    CHashTable::addObject(&firstPair);
	
    while (true) {
        va_list tmpList;
		va_copy(tmpList, argList);
        if (NIL_OBJ == va_arg(tmpList, void*)) break;
        OBJ_TYPE object = va_arg(argList, OBJ_TYPE);
        KEY_TYPE key = va_arg(argList, KEY_TYPE);
        SPair pair(&object, &key);
        CHashTable::addObject(&pair);
    }
    va_end(argList);
}
*/

_DECLARE_DICT_TEMPLATE
_CLASS_DICT_TEMPLATE::CDictT(const OBJ_TYPE* cObjectsArray, const KEY_TYPE* cKeysArray, dInteger count) {
	CL_THROW_IF_TRUE(!cObjectsArray || !cKeysArray || 0 == count);

    SPair* pairs = new SPair[count];

    try {
        for (dInteger i = 0; i < count; ++i) {
            pairs[i].object = const_cast<OBJ_TYPE*>(cObjectsArray++);
            pairs[i].key = const_cast<KEY_TYPE*>(cKeysArray++);
        }

        CHashTable::initWithCArray(sizeof(SPair), pairs, count);
    } catch (...) {
        SAFE_DELETE_ARRAY(pairs);
        throw;
    }
    SAFE_DELETE_ARRAY(pairs);
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::allKeys(CArrayT<KEY_TYPE, ARG_KEY_TYPE>* outKeys) const {
	CL_THROW_IF_NIL(outKeys);
    outKeys->removeAllObjects();

    Iterator iter(const_cast<_CLASS_DICT_TEMPLATE*>(this));
    while (iter.next()) outKeys->addObject(iter.key());
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::allKeysForObject(CArrayT<KEY_TYPE, ARG_KEY_TYPE>* outKeys, ARG_OBJ_TYPE anObject) const {
	CL_THROW_IF_NIL(outKeys);
    outKeys->removeAllObjects();

    SPair objPair(&anObject, (KEY_TYPE*)NIL_OBJ);
    Iterator iter(const_cast<_CLASS_DICT_TEMPLATE*>(this));
    while (iter.next()) {
        ARG_OBJ_TYPE objRef = iter.object();
        SPair iterPair(&objRef, (KEY_TYPE*)NIL_OBJ);
        if (kCompareEqualTo == _compare(&objPair, &iterPair))
            outKeys->addObject(iter.key());
    }
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::allValues(CArrayT<OBJ_TYPE, ARG_OBJ_TYPE>* outObjects) const {
	CL_THROW_IF_NIL(outObjects);
    outObjects->removeAllObjects();

    Iterator iter(const_cast<_CLASS_DICT_TEMPLATE*>(this));
    while (iter.next()) outObjects->addObject(iter.object());
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::getObjectsAndKeys(OBJ_TYPE* cObjectArray, KEY_TYPE* cKeyArray) const {
	CL_THROW_IF_TRUE(!cObjectArray || !cKeyArray);

    Iterator iter(const_cast<_CLASS_DICT_TEMPLATE*>(this));
    while (iter.next()) {
        new (cObjectArray++) OBJ_TYPE(iter.object());
        new (cKeyArray++) KEY_TYPE(iter.key());
    }
}

_DECLARE_DICT_TEMPLATE
ARG_OBJ_TYPE
_CLASS_DICT_TEMPLATE::objectForKey(ARG_KEY_TYPE key) const {
    SPair pair(&key);
    SPair* resultPair = (SPair*)CHashTable::objectForKey(&pair);

    if (!resultPair) throw new ExDictObjectNotFound(key);

    return *resultPair->object;
}

_DECLARE_DICT_TEMPLATE
ARG_OBJ_TYPE
_CLASS_DICT_TEMPLATE::objectForKeyNoException(ARG_KEY_TYPE key) const {
    SPair pair(&key);
    SPair* resultPair = (SPair*)CHashTable::objectForKey(&pair);
    
    if (!resultPair)
        return nil;
    return *resultPair->object;
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::objectsForKeysNotFoundMarker(
    CArrayT<OBJ_TYPE, ARG_OBJ_TYPE>* outObjects,
    const CArrayT<KEY_TYPE, ARG_KEY_TYPE>* keys,
    ARG_OBJ_TYPE notFoundMarker) const {

	CL_THROW_IF_TRUE(!outObjects || !keys);
    outObjects->removeAllObjects();

    for (dInteger i = 0, cnt = keys->count(); i < cnt; ++i) {
        try {
            outObjects->addObject(objectForKey(keys->objectAtIndex(i)));
        } catch (ExDictObjectNotFound* e) {
            outObjects->addObject(notFoundMarker);
            SAFE_DELETE(e);
        }
    }
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::setObjectForKey(ARG_OBJ_TYPE object, ARG_KEY_TYPE key) {
    SPair pair(&object, &key);
    CHashTable::addObject(&pair);
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::removeObjectForKey(ARG_KEY_TYPE key) {
    SPair pair(&key);
    CHashTable::removeObject(&pair);
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::removeObjectsForKeys(CArrayT<KEY_TYPE, ARG_KEY_TYPE>* keyArray) {
    for (dInteger i = 0, cnt = keyArray->count(); i < cnt; ++i)
        removeObject(keyArray->objectAtIndex(i));
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::_copyItem(void* dst, const void* src) const {
    COPY_DELETE_FUNCS<OBJ_TYPE, KEY_TYPE, SPair>::copyItem((SPair*)dst, (SPair*)src);
}

_DECLARE_DICT_TEMPLATE
void
_CLASS_DICT_TEMPLATE::_destroyItem(void* item) const {
    COPY_DELETE_FUNCS<OBJ_TYPE, KEY_TYPE, SPair>::destroyItem((SPair*)item);
}

_DECLARE_DICT_TEMPLATE
dInteger
_CLASS_DICT_TEMPLATE::_hash(const void* object) const {
    return HASH_EQUAL_FUNCS<OBJ_TYPE, KEY_TYPE>::hashValueForKey(((SPair*)object)->key);
}

_DECLARE_DICT_TEMPLATE
dComparisonResult
_CLASS_DICT_TEMPLATE::_isEqualKeys(const void* obj1, const void* obj2) const {
    return (HASH_EQUAL_FUNCS<OBJ_TYPE, KEY_TYPE>::isEqualKeys(
                    ((SPair*)obj1)->key,
                    ((SPair*)obj2)->key,
                    _comparatorContext()))
        ? kCompareEqualTo
        : !kCompareEqualTo;
}

_DECLARE_DICT_TEMPLATE
dComparisonResult
_CLASS_DICT_TEMPLATE::_compare(const void* obj1, const void* obj2) const {
    return (HASH_EQUAL_FUNCS<OBJ_TYPE, KEY_TYPE>::isEqualObjects(
                    ((SPair*)obj1)->object,
                    ((SPair*)obj2)->object,
                    _comparatorContext()))
        ? kCompareEqualTo
        : !kCompareEqualTo;
}

END_NAMESPACE_CL

#endif // _COCOA_CDICT_H