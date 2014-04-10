/*
** $Id: 2009/04/04, Taras Tovchenko $
** Uses for calling selector-methods, and for keep commands
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DINVOCATION_H
#define _COCOA_DINVOCATION_H



BEGIN_NAMESPACE_CL


class DLL_PUBLIC dInvocation : public dObject {
CL_CLASS_DECL(dInvocation, dObject)
public:
    virtual dInvocation*        initWithParams(dObject* target, dSEL sel, /* dObject*, ..., nil */ ...);
    virtual dInvocation*        initWithVAList(dObject* target, dSEL sel, va_list argList);

    dSEL                        selector() const { return _selector; }
    void                        setSelector(dSEL sel) { _selector = sel; }
    dObject*                    target() const { return _target; }
    void                        setTarget(dObject* targ) { CL_SWITCH_LINKS(_target, targ); }

    void                        invokeWithTarget(dObject* target);
    void                        invoke();

    dObject*                    returnValue() const;

protected:
    dInvocation();
    virtual void                dealloc();

    dObject*                    _target;
    dSEL                        _selector;
    dArray*                     _args;
    dObject*                    _returnValue;
    
    dObjectMethodPtr            _methodCache;
    dObject*                    _targetCache;
};

END_NAMESPACE_CL

#endif // _COCOA_DINVOCATION_H