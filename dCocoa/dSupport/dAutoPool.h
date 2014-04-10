/*
** $Id: 2009/03/23, Taras Tovchenko $
** Part at memory menagement, give you possibility to put your d-objects
** on pool which will be automatic release its.
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DAUTOPOOL_H
#define _COCOA_DAUTOPOOL_H


#define _LocalPool  CL::dobj_ptr<CL::dAutoPool> __pool(CL_NEW(CL::dAutoPool));



BEGIN_NAMESPACE_CL
template <class TYPE> class dArrayT;

class DLL_PUBLIC dAutoPool : public dObject {
CL_CLASS_DECL(dAutoPool, dObject)
public:
    virtual dAutoPool*          init();

    static dAutoPool* PASCAL    currentPool();
    static dAutoPool* PASCAL    topPool();

    void                        addObject(const dObject* obj);

protected:
    dAutoPool();
    virtual void                dealloc();

    dArrayT<dArray*>*           _pages;
    dAutoPool*                  _parentPool;
    dAutoPool*                  _childPool;
};

END_NAMESPACE_CL

#endif // _COCOA_DAUTOPOOL_H