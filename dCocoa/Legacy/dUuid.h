/*
** $Id: 2009/08/03, Taras Tovchenko $
** unique identifier, objects can use it for identification or/and saving 
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DUUID_H
#define _COCOA_DUUID_H


#define kUuidLength 16


BEGIN_NAMESPACE_CL


class DLL_PUBLIC dUuid : public dObject {
CL_CLASS_DECL(dUuid, dObject)
public:
    static dUuid*               uniqueId() { _self* u = _self::alloc()->initWithUniqueId(); return u ? u->autorelease() : nil; }
    virtual dUuid*              initWithUniqueId();
    // hash & equal
    virtual dInteger            hash() const;
    virtual bool                isEqual(const dObject* otherObject) const;

protected:
    dUuid();

    dByte                       _uuid[kUuidLength];
};

END_NAMESPACE_CL

#endif // _COCOA_DUUID_H