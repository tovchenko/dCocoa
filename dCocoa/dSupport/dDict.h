/*
** $Id: 2009/03/20, Taras Tovchenko $
** Array which contains dObjects and correct work with its.
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DDICT_H
#define _COCOA_DDICT_H

#include "CDictT.h"


BEGIN_NAMESPACE_CL

template <typename OBJ_TYPE, typename KEY_TYPE>
class CHashEqualCocoaDictPolicyT {
public:
    static dInteger     hashValueForKey(const KEY_TYPE* k) { return (*k)->hash(); }
    static bool         isEqualKeys(const KEY_TYPE* k1, const KEY_TYPE* k2, void* context) { return (*k1)->isEqual(*k2); }
    static bool         isEqualObjects(const OBJ_TYPE* o1, const OBJ_TYPE* o2, void* context) { return (*o1)->isEqual(*o2); }
};

template <typename OBJ_TYPE, typename KEY_TYPE, typename PAIR_TYPE>
class CCopyDeleteCocoaDictPolicyT {
public:
    static void     copyItem(PAIR_TYPE* dst, const PAIR_TYPE* src) {
        dst->object = new OBJ_TYPE(const_cast<OBJ_TYPE>((*src->object)->const_retain()));
        dst->key = new KEY_TYPE(const_cast<KEY_TYPE>((*src->key)->const_retain()));
    }

    static void     destroyItem(PAIR_TYPE* item) {
        (*item->object)->release();
        SAFE_DELETE(item->object);
        (*item->key)->release();
        SAFE_DELETE(item->key);
    }
};


template <typename dOBJ_TYPE, typename dKEY_TYPE>
class dDictT : public dObject,
               public CDictT<
                   dOBJ_TYPE, dKEY_TYPE,
                   CHashEqualCocoaDictPolicyT,
                   dOBJ_TYPE, dKEY_TYPE,
                   CCopyDeleteCocoaDictPolicyT> {
typedef dDictT<dOBJ_TYPE, dKEY_TYPE>    _self;
CL_CLASS_TEMPLATE(_self, dObject)
public:

};

class DLL_PUBLIC dDict : public dDictT<dObject*, dObject*> {
typedef dDictT<dObject*, dObject*> _baseType;
CL_CLASS_DECL(dDict, _baseType)
};

END_NAMESPACE_CL

#endif // _COCOA_DDICT_H