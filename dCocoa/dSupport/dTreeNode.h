/*
** $Id: 2009/08/02, Taras Tovchenko $
** TreeNode for dObjects
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DTREENODE_H
#define _COCOA_DTREENODE_H

#include "CTreeNodeT.h"



BEGIN_NAMESPACE_CL


template <typename TYPE>
class CDataOwnershipCocoaPolicyT {
public:
    static void     copyItem(TYPE* dst, TYPE* src) {
        CL_SWITCH_LINKS(*dst, *src);
    }

    static void     destroyItem(TYPE* dst) {
        CL_RELEASE(*dst);
    }
};

template <typename TYPE>
class CNodeOwnershipCocoaPolicyT {
public:
    static void     attach(TYPE* node) {
        CL_RETAIN(node);
    }

    static void     detach(TYPE* node) {
        CL_RELEASE(node);
    }
};


template <
    typename TYPE,
    template <typename> class COPY_DELETE_FUNCS = CDataOwnershipCocoaPolicyT>
class dTreeNodeT : public dObject,
                   public CTreeNodeImplT<
                            TYPE,
                            TYPE,
                            COPY_DELETE_FUNCS,
                            CNodeOwnershipCocoaPolicyT,
                            dTreeNodeT<TYPE, COPY_DELETE_FUNCS> > {
typedef dTreeNodeT<TYPE, COPY_DELETE_FUNCS> _self;
CL_CLASS_TEMPLATE(_self, dObject)
typedef dObject _super;
public:
    virtual dTreeNodeT<TYPE, COPY_DELETE_FUNCS>* initWithValue(TYPE val) {
        if (!_super::init()) return nil;
        CTreeNodeImplT<
            TYPE,
            TYPE,
            COPY_DELETE_FUNCS,
            CNodeOwnershipCocoaPolicyT,
            dTreeNodeT<TYPE, COPY_DELETE_FUNCS> >::setValue(val);
        
        return this;
    }
};


class DLL_PUBLIC dTreeNode : public dTreeNodeT<dObject*> {
CL_CLASS_DECL(dTreeNode, dTreeNodeT<dObject*>)
};

END_NAMESPACE_CL

#endif // _COCOA_DTREENODE_H