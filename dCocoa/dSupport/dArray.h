/*
** $Id: 2009/03/18, Taras Tovchenko $
** Array which contains dObjects and correct work with its.
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DARRAY_H
#define _COCOA_DARRAY_H

#include "CArrayT.h"


BEGIN_NAMESPACE_CL

template <typename TYPE>
class CCopyDeleteCocoaArrayPolicyT {
public:
    static void     copyItems(TYPE* dst, const TYPE* src, dInteger count) {
        while (count--) {
            *dst++ = const_cast<TYPE>((*src++)->const_retain());
        }
    }

    static void     destroyItems(TYPE* objPtr, dInteger count) {
        if (!objPtr) return;
        for (dInteger i = 0; i < count; ++i)
            (*(objPtr + i))->release();
    }
};


template <typename dTYPE>
class dArrayT : public dObject,
                public CArrayT<dTYPE, dTYPE, CCopyDeleteCocoaArrayPolicyT> {
CL_CLASS_TEMPLATE(dArrayT<dTYPE>, dObject)

};

class DLL_PUBLIC dArray : public dArrayT<dObject*> {
CL_CLASS_DECL(dArray, dArrayT<dObject*>)
public:

};

END_NAMESPACE_CL

#endif // _COCOA_DARRAY_H