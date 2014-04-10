/*
** $Id: 2009/08/01, Taras Tovchenko $
** Represent the node which can be connected to other nodes
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_CTREENODET_H
#define _COCOA_CTREENODET_H

#include "CArrayT.h"


#define _DECLARE_TREENODE_TEMPLATE template <    \
                            typename TYPE,    \
                            typename ARG_TYPE,    \
                            template <typename> class COPY_DELETE_FUNCS,    \
                            template <typename> class NODE_OWNERSHIP_FUNCS,    \
                            typename NODE_TYPE>

#define _CLASS_TREENODE_TEMPLATE CTreeNodeImplT<    \
                            TYPE,    \
                            ARG_TYPE,    \
                            COPY_DELETE_FUNCS,    \
                            NODE_OWNERSHIP_FUNCS,    \
                            NODE_TYPE>

#define _CTREENODE_TYPE CTreeNodeT<    \
                            TYPE,    \
                            ARG_TYPE,    \
                            COPY_DELETE_FUNCS,    \
                            NODE_OWNERSHIP_FUNCS>

#define _CARRAY_TYPE		CArrayT<    \
                                NODE_TYPE*,    \
                                NODE_TYPE*,    \
                                CCopyDeleteClassicArrayPolicyT>

#define _CTREENODEIMPL_TYPE	CTreeNodeImplT<    \
                                TYPE,    \
                                ARG_TYPE,    \
                                COPY_DELETE_FUNCS,    \
                                NODE_OWNERSHIP_FUNCS,    \
                                _CTREENODE_TYPE>    \

BEGIN_NAMESPACE_CL


template <typename TYPE>
class CDataOwnershipClassicPolicyT {
public:
    static void     copyItem(TYPE* dst, TYPE* src) {
        *dst = *src;
    }

    static void     destroyItem(TYPE* dst) {
    }
};

template <typename TYPE>
class CNodeOwnershipClassicPolicyT {
public:
    static void     attach(TYPE* node) {
    }

    static void     detach(TYPE* node) {
        SAFE_DELETE(node);
    }
};


// BE CAREFULL: only root node can be created on a stack,
// all others nodes must be created in dynamic memory

template <
    typename TYPE,
    typename ARG_TYPE,
    template <typename> class COPY_DELETE_FUNCS,
    template <typename> class NODE_OWNERSHIP_FUNCS,
    typename NODE_TYPE>
class CTreeNodeImplT : private _CARRAY_TYPE {
public:
    CTreeNodeImplT();
    CTreeNodeImplT(ARG_TYPE data);
    virtual ~CTreeNodeImplT();
    // add / remove
    void                        addChild(NODE_TYPE* node);
    void                        insertChildAtIndex(NODE_TYPE* node, dInteger index);
    void                        removeChild(NODE_TYPE* node);
    void                        removeChildAtIndex(dInteger index);
    void                        removeAllChildren();
    // when node was detaching you got ownership to it
    void                        detachChild(NODE_TYPE* node);
    NODE_TYPE*                  detachChildAtIndex(dInteger index);

    NODE_TYPE*                  topNode() const { return _parent ? _parent->topNode() : (NODE_TYPE*)this; }
    NODE_TYPE*                  parent() const { return _parent; }
    dInteger                    childrenCount() const { return _CARRAY_TYPE::count(); }
    NODE_TYPE*                  childAtIndex(dInteger index) const { return _CARRAY_TYPE::objectAtIndex(index); }
    dInteger                    indexOfChild(NODE_TYPE* node) const { return _CARRAY_TYPE::indexOfObject(node); }

    ARG_TYPE                    value() { return _data; }
    void                        setValue(ARG_TYPE val);

protected:
    // override
    virtual void                _willDetach() { };
    virtual void                _didDetach() { };
    
    TYPE                        _data;
    NODE_TYPE*                  _parent;
};


template <
    typename TYPE,
    typename ARG_TYPE = TYPE&,
    template <typename> class COPY_DELETE_FUNCS = CDataOwnershipClassicPolicyT,
    template <typename> class NODE_OWNERSHIP_FUNCS = CNodeOwnershipClassicPolicyT>
class CTreeNodeT : public _CTREENODEIMPL_TYPE {
public:
    CTreeNodeT() : _CTREENODEIMPL_TYPE() { }
    CTreeNodeT(ARG_TYPE data) : _CTREENODEIMPL_TYPE(data) { }

private:
    // forbidden
    CTreeNodeT(const _CTREENODE_TYPE& rf);
    const _CTREENODE_TYPE& operator=(const _CTREENODE_TYPE& rf);
};


_DECLARE_TREENODE_TEMPLATE
_CLASS_TREENODE_TEMPLATE::CTreeNodeImplT()
: _data(nil),
  _parent(nil) {
}

_DECLARE_TREENODE_TEMPLATE
_CLASS_TREENODE_TEMPLATE::CTreeNodeImplT(ARG_TYPE data)
: _data(nil),
  _parent(nil) {
    setValue(data);
}

_DECLARE_TREENODE_TEMPLATE
_CLASS_TREENODE_TEMPLATE::~CTreeNodeImplT() {
    COPY_DELETE_FUNCS<TYPE>::destroyItem(&_data);
    removeAllChildren();
}

_DECLARE_TREENODE_TEMPLATE
void
_CLASS_TREENODE_TEMPLATE::addChild(NODE_TYPE* node) {
    insertChildAtIndex(node, _CARRAY_TYPE::count());
}

_DECLARE_TREENODE_TEMPLATE
void
_CLASS_TREENODE_TEMPLATE::insertChildAtIndex(NODE_TYPE* node, dInteger index) {
    if (!node) return;
    NODE_OWNERSHIP_FUNCS<NODE_TYPE>::attach(node);
    if (node->_parent) _parent->detachChild(node);
    _CARRAY_TYPE::insertObjectAtIndex(node, index);
    node->_parent = (NODE_TYPE*)this;
}

_DECLARE_TREENODE_TEMPLATE
void
_CLASS_TREENODE_TEMPLATE::removeChild(NODE_TYPE* node) {
    detachChild(node);
    NODE_OWNERSHIP_FUNCS<NODE_TYPE>::detach(node);
}

_DECLARE_TREENODE_TEMPLATE
void
_CLASS_TREENODE_TEMPLATE::removeChildAtIndex(dInteger index) {
    NODE_OWNERSHIP_FUNCS<NODE_TYPE>::detach(detachChildAtIndex(index));
}

_DECLARE_TREENODE_TEMPLATE
void
_CLASS_TREENODE_TEMPLATE::removeAllChildren() {
    for (dInteger i = _CARRAY_TYPE::count() - 1; i >= 0; --i)
        removeChildAtIndex(i);
}

_DECLARE_TREENODE_TEMPLATE
void
_CLASS_TREENODE_TEMPLATE::detachChild(NODE_TYPE* node) {
    if (node) {
        _willDetach();
        _CARRAY_TYPE::removeObject(node);
        node->_parent = nil;
        _didDetach();
    }
}

_DECLARE_TREENODE_TEMPLATE
NODE_TYPE*
_CLASS_TREENODE_TEMPLATE::detachChildAtIndex(dInteger index) {
    NODE_TYPE* node = _CARRAY_TYPE::objectAtIndex(index);
    
    _willDetach();
    _CARRAY_TYPE::removeObjectAtIndex(index);
    node->_parent = nil;
    _didDetach();
    
    return node;
}

_DECLARE_TREENODE_TEMPLATE
void
_CLASS_TREENODE_TEMPLATE::setValue(ARG_TYPE val) {
    if (_data == val) return;
    COPY_DELETE_FUNCS<TYPE>::destroyItem(&_data);
    COPY_DELETE_FUNCS<TYPE>::copyItem(&_data, &val);
}

END_NAMESPACE_CL

#endif // _COCOA_CTREENODET_H