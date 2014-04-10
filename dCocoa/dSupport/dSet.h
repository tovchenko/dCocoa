/*
** $Id: 2009/03/19, Taras Tovchenko $
** Set which contains dObjects and correct work with its.
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DSET_H
#define _COCOA_DSET_H

#include "CSetT.h"


BEGIN_NAMESPACE_CL

template <typename TYPE>
class CHashEqualCocoaSetPolicyT {
public:
    static dInteger     hashValueForKey(const TYPE* key) { return (*key)->hash(); }
    static bool         isEqualKeys(const TYPE* key1, const TYPE* key2, void* context) { return (*key1)->isEqual(*key2); }
};

template <typename TYPE, typename CTX_TYPE>
class CCopyDeleteCocoaSetPolicyT {
public:
    static void     copyItem(TYPE* dst, const TYPE* src, CTX_TYPE ctx) {
        *dst = const_cast<TYPE>((*src)->const_retain());
    }

    static void     destroyItem(TYPE* item, CTX_TYPE ctx) {
        (*item)->release();
    }
};


template <typename dTYPE>
class dSetT : public dObject,
              public CSetT<dTYPE, CHashEqualCocoaSetPolicyT, dTYPE, CCopyDeleteCocoaSetPolicyT> {
CL_CLASS_TEMPLATE(dSetT<dTYPE>, dObject)
public:

};

class DLL_PUBLIC dSet : public dSetT<dObject*> {
CL_CLASS_DECL(dSet, dSetT<dObject*>)
};

END_NAMESPACE_CL

#endif // _COCOA_DSET_H