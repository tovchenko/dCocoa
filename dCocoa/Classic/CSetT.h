/*
** $Id: 2009/02/07, Taras Tovchenko $
** Real template set class which use private realization of CHashTable
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_CSETT_H
#define _COCOA_CSETT_H

#include "CHashTable.h"

#include <memory>



#define _DECLARE_SET_TEMPLATE template <    \
                        typename TYPE,    \
                        template <typename> class HASH_EQUAL_FUNCS,    \
                        typename ARG_TYPE,    \
                        template <typename, typename> class COPY_DELETE_FUNCS,    \
                        typename COPY_DELETE_CTX_TYPE>

#define _CLASS_SET_TEMPLATE CSetT<    \
                        TYPE,    \
                        HASH_EQUAL_FUNCS,    \
                        ARG_TYPE,    \
                        COPY_DELETE_FUNCS,    \
                        COPY_DELETE_CTX_TYPE>


BEGIN_NAMESPACE_CL

template <typename TYPE, typename CTX_TYPE>
class CCopyDeleteClassicSetPolicyT {
public:
    static void     copyItem(TYPE* dst, const TYPE* src, CTX_TYPE ctx) {
        new (dst) TYPE(*src);
    }

    static void     destroyItem(TYPE* item, CTX_TYPE ctx) {
        item->~TYPE();
    }
};


template <
    typename TYPE,
    template <typename> class HASH_EQUAL_FUNCS,
    typename ARG_TYPE = TYPE&,
    template <typename, typename> class COPY_DELETE_FUNCS = CCopyDeleteClassicSetPolicyT,
    typename COPY_DELETE_CTX_TYPE = void*>
class CSetT : private CHashTable {
public:
    // exceptions
    class ExSetObjectNotFound : public CException {
    public:
        ExSetObjectNotFound() { }
        ExSetObjectNotFound(ARG_TYPE notFoundKey) {
            _key = std::auto_ptr<TYPE>(new TYPE(notFoundKey));
        }
        ARG_TYPE        notFoundKey() { return *_key.get(); }

    protected:
        std::auto_ptr<TYPE>    _key;
    };
    // iterating
    class Iterator {
    public:
        Iterator(_CLASS_SET_TEMPLATE* set) : _iter(set) { }

        void            reset() { _iter.reset(); }
        bool            next() { return _iter.next(); }
        ARG_TYPE        object() const { return *(TYPE*)_iter.object(); }

    protected:
        CHashTable::Iterator    _iter;
    };
    // interface
//  CSetT(TYPE first, ...); // must end of NIL_OBJ, this method dont work on mac 64-bit 
    CSetT(dInteger capacity) { CHashTable::initWithCapacity(sizeof(TYPE), capacity); }
    CSetT(CArrayT<TYPE, ARG_TYPE>* anArray) { CHashTable::initWithDefaults(sizeof(TYPE)); addObjectsFromArray(anArray); }
    CSetT() { CHashTable::initWithDefaults(sizeof(TYPE)); }
    CSetT(const TYPE* objects, dInteger count) { CHashTable::initWithCArray(sizeof(TYPE), objects, count); }
    CSetT(const _CLASS_SET_TEMPLATE* otherSet) { CHashTable::initWithHashTable(otherSet); }
    CSetT(const _CLASS_SET_TEMPLATE& otherSet) { CHashTable::initWithHashTable(&otherSet); }
    virtual ~CSetT() { CHashTable::_dealloc(); }
    // counting entries
    dInteger                    count() const { return CHashTable::count(); }
    // accessing set members
    void                        allObjects(CArrayT<TYPE, ARG_TYPE>* outArray) const;
    ARG_TYPE                    anyObject() const;
    bool                        containsObject(ARG_TYPE object) const { return CHashTable::objectForKey(&object) ? true : false; }
    ARG_TYPE                    member(ARG_TYPE object) const;
    // adding and removing entries
    void                        addObject(ARG_TYPE object) { CHashTable::addObject(&object); }
    void                        removeObject(ARG_TYPE object) { CHashTable::removeObject(&object); }
    void                        removeAllObjects() { CHashTable::removeAllObjects(); }
    void                        addObjectsFromArray(const CArrayT<TYPE, ARG_TYPE>* anArray);
    // combining and recombining
    void                        unionSet(const _CLASS_SET_TEMPLATE* otherSet);
    void                        minusSet(const _CLASS_SET_TEMPLATE* otherSet);
    void                        intersectSet(const _CLASS_SET_TEMPLATE* otherSet);
    void                        setSet(const _CLASS_SET_TEMPLATE* otherSet);
    // comparing sets
    bool                        isSubsetOfSet(const _CLASS_SET_TEMPLATE* otherSet) const;
    bool                        intersectsSet(const _CLASS_SET_TEMPLATE* otherSet) const;
    bool                        isEqualToSet(const _CLASS_SET_TEMPLATE* otherSet) const;
    // deriving new set, this methods return allocated set
    // you must deallocate it when finish work with it
    _CLASS_SET_TEMPLATE*        newSetByAddingObject(ARG_TYPE object) const;
    _CLASS_SET_TEMPLATE*        newSetByAddingObjectsFromSet(const _CLASS_SET_TEMPLATE* other) const;
    _CLASS_SET_TEMPLATE*        newSetByAddingObjectsFromArray(const CArrayT<TYPE, ARG_TYPE>* other) const;
    // custom copy-destroy object's context managenent
    COPY_DELETE_CTX_TYPE        copyDestroyContext() const { return _copyDestroyContext; }
    void                        setCopyDestroyContext(COPY_DELETE_CTX_TYPE ctx) { _copyDestroyContext = ctx; }

protected:
    virtual void                _copyItem(void* dst, const void* src) const;
    virtual void                _destroyItem(void* item) const;
    // hashing
    virtual dInteger            _hash(const void* object) const;
    virtual dComparisonResult   _isEqualKeys(const void* obj1, const void* obj2) const;

    COPY_DELETE_CTX_TYPE        _copyDestroyContext;
};

/*
_DECLARE_SET_TEMPLATE
_CLASS_SET_TEMPLATE::CSetT(TYPE first, ...) {
    CHashTable::initWithDefaults(sizeof(TYPE));
    CHashTable::addObject(&first);

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
        
        TYPE obj = va_arg(argList, TYPE);
        CHashTable::addObject(&obj);
    }
    va_end(argList);
}
*/

_DECLARE_SET_TEMPLATE
void
_CLASS_SET_TEMPLATE::allObjects(CArrayT<TYPE, ARG_TYPE>* outArray) const {
	CL_THROW_IF_NIL(outArray);
    outArray->removeAllObjects();

    Iterator iter(const_cast<_CLASS_SET_TEMPLATE*>(this));
    while (iter.next()) outArray->addObject(iter.object());
}

_DECLARE_SET_TEMPLATE
ARG_TYPE
_CLASS_SET_TEMPLATE::anyObject() const {
    if (0 == _count) throw new ExSetObjectNotFound;

    Iterator iter(const_cast<_CLASS_SET_TEMPLATE*>(this));
    iter.next();
    return iter.object();
}

_DECLARE_SET_TEMPLATE
ARG_TYPE
_CLASS_SET_TEMPLATE::member(ARG_TYPE object) const {
    TYPE* result = (TYPE*)CHashTable::objectForKey(&object);
    if (!result) throw new ExSetObjectNotFound(object);

    return *result;
}

_DECLARE_SET_TEMPLATE
void
_CLASS_SET_TEMPLATE::addObjectsFromArray(const CArrayT<TYPE, ARG_TYPE>* anArray) {
	CL_THROW_IF_NIL(anArray);

    for (dInteger i = 0, cnt = anArray->count(); i < cnt; ++i)
        addObject(anArray->objectAtIndex(i));
}

_DECLARE_SET_TEMPLATE
void
_CLASS_SET_TEMPLATE::unionSet(const _CLASS_SET_TEMPLATE* otherSet) {
	CL_THROW_IF_NIL(otherSet);
    if (0 == otherSet->_count) return;

    Iterator iter(const_cast<_CLASS_SET_TEMPLATE*>(otherSet));
    while (iter.next()) addObject(iter.object());
}

_DECLARE_SET_TEMPLATE
void
_CLASS_SET_TEMPLATE::minusSet(const _CLASS_SET_TEMPLATE* otherSet) {
	CL_THROW_IF_NIL(otherSet);
    if (0 == otherSet->_count) return;

    Iterator iter(const_cast<_CLASS_SET_TEMPLATE*>(otherSet));
    while (iter.next()) removeObject(iter.object());
}

_DECLARE_SET_TEMPLATE
void
_CLASS_SET_TEMPLATE::intersectSet(const _CLASS_SET_TEMPLATE* otherSet) {
	CL_THROW_IF_NIL(otherSet);
    if (0 == otherSet->_count) {
        removeAllObjects();
        return;
    }

    Iterator iter(const_cast<_CLASS_SET_TEMPLATE*>(this));
    while (iter.next())
        if (!otherSet->containsObject(iter.object()))
            removeObject(iter.object());
}

_DECLARE_SET_TEMPLATE
void
_CLASS_SET_TEMPLATE::setSet(const _CLASS_SET_TEMPLATE* otherSet) {
	CL_THROW_IF_NIL(otherSet);
    removeAllObjects();
    if (0 == otherSet->_count) return;

    unionSet(otherSet);
}

_DECLARE_SET_TEMPLATE
bool
_CLASS_SET_TEMPLATE::isSubsetOfSet(const _CLASS_SET_TEMPLATE* otherSet) const {
	CL_THROW_IF_NIL(otherSet);

    Iterator iter(const_cast<_CLASS_SET_TEMPLATE*>(this));
    while (iter.next())
        if (!otherSet->containsObject(iter.object()))
            return false;
    return true;
}

_DECLARE_SET_TEMPLATE
bool
_CLASS_SET_TEMPLATE::intersectsSet(const _CLASS_SET_TEMPLATE* otherSet) const {
	CL_THROW_IF_NIL(otherSet);
    if (0 == otherSet->_count) return false;

    Iterator iter(const_cast<_CLASS_SET_TEMPLATE*>(this));
    while (iter.next())
        if (otherSet->containsObject(iter.object()))
            return true;
    return false;
}

_DECLARE_SET_TEMPLATE
bool
_CLASS_SET_TEMPLATE::isEqualToSet(const _CLASS_SET_TEMPLATE* otherSet) const {
	CL_THROW_IF_NIL(otherSet);
    if (_count != otherSet->_count) return false;

    Iterator iter(const_cast<_CLASS_SET_TEMPLATE*>(otherSet));
    while (iter.next())
        if (!containsObject(iter.object()))
            return false;
    return true;
}

_DECLARE_SET_TEMPLATE
_CLASS_SET_TEMPLATE*
_CLASS_SET_TEMPLATE::newSetByAddingObject(ARG_TYPE object) const {
    _CLASS_SET_TEMPLATE* result = new _CLASS_SET_TEMPLATE(this);

    result->addObject(object);
    return result;
}

_DECLARE_SET_TEMPLATE
_CLASS_SET_TEMPLATE*
_CLASS_SET_TEMPLATE::newSetByAddingObjectsFromSet(const _CLASS_SET_TEMPLATE* other) const {
    _CLASS_SET_TEMPLATE* result = new _CLASS_SET_TEMPLATE(this);

    result->unionSet(other);
    return result;
}

_DECLARE_SET_TEMPLATE
_CLASS_SET_TEMPLATE*
_CLASS_SET_TEMPLATE::newSetByAddingObjectsFromArray(const CArrayT<TYPE, ARG_TYPE>* other) const {
    _CLASS_SET_TEMPLATE* result = new _CLASS_SET_TEMPLATE(this);

    result->addObjectsFromArray(other);
    return result;
}

_DECLARE_SET_TEMPLATE
void
_CLASS_SET_TEMPLATE::_copyItem(void* dst, const void* src) const {
    COPY_DELETE_FUNCS<TYPE, COPY_DELETE_CTX_TYPE>::copyItem((TYPE*)dst, (const TYPE*)src, _copyDestroyContext);
}

_DECLARE_SET_TEMPLATE
void
_CLASS_SET_TEMPLATE::_destroyItem(void* item) const {
    COPY_DELETE_FUNCS<TYPE, COPY_DELETE_CTX_TYPE>::destroyItem((TYPE*)item, _copyDestroyContext);
}

_DECLARE_SET_TEMPLATE
dInteger
_CLASS_SET_TEMPLATE::_hash(const void* object) const {
    return HASH_EQUAL_FUNCS<TYPE>::hashValueForKey((TYPE*)object);
}

_DECLARE_SET_TEMPLATE
dComparisonResult
_CLASS_SET_TEMPLATE::_isEqualKeys(const void* obj1, const void* obj2) const {
    return (HASH_EQUAL_FUNCS<TYPE>::isEqualKeys(
                    (TYPE*)obj1,
                    (TYPE*)obj2,
                    _comparatorContext()))
            ? kCompareEqualTo
            : !kCompareEqualTo;
}

END_NAMESPACE_CL

#endif // _COCOA_CSETT_H