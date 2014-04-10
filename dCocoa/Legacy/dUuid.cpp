/*
** $Id: 2009/08/03, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dUuid.h"

#if defined (IS_WIN)
    #include <ObjBase.h>
#elif defined (IS_MAC)
    #include <uuid/uuid.h>
#endif


static void     _generateUuid(dByte* outBuffer);

CL_CLASS_DEF(dUuid, dCocoa)


dUuid::dUuid() {
    ZERO_MEMORY_ARRAY(_uuid);
}

dUuid*
dUuid::initWithUniqueId() {
    if (!_super::init()) return nil;
    _generateUuid(_uuid);
    return this;
}

dInteger
dUuid::hash() const {
    return uALGO::hashFromByteSequence(_uuid, kUuidLength);
}

bool
dUuid::isEqual(const dObject* otherObject) const {
    return this == otherObject || !std::memcmp(_uuid, ((_self*)otherObject)->_uuid, kUuidLength);
}


static void
_generateUuid(dByte* outBuffer) {
#if defined (IS_WIN)
    GUID u;
    CoCreateGuid(&u);
    memcpy_s(outBuffer, kUuidLength, &u, kUuidLength);
#elif defined (IS_MAC)
    uuid_t u;
    uuid_generate(u);
    std::memcpy(outBuffer, u, kUuidLength);
#endif
}