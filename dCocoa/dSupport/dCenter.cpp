/*
** $Id: 2009/07/30, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dCenter.h"

#include "dString.h"
#include "dData.h"
#include "dDict.h"
#include "dArray.h"

CL_CLASS_DEF(dNotification, dCocoa)


dNotification::dNotification()
: _object(nil),
  _name(nil),
  _info(nil) {
}

dNotification*
dNotification::notificationWithParams(dObject* object, const dString* name, dDict* userInfo) {
    dNotification* ntf = alloc()->initWithParams(object, name, userInfo);
    return ntf ? ntf->autorelease() : nil;
}

dNotification*
dNotification::initWithParams(dObject* object, const dString* name, dDict* userInfo) {
    if (!_super::init()) return nil;
	CL_THROW_IF_TRUE(!object || !name);

    _object = object->retain();
    _name = const_cast<dString*>(name);
    _name->retain();
    _info = userInfo ? userInfo->retain() : nil;

    return this;
}

void
dNotification::dealloc() {
    CL_RELEASE(_object);
    CL_RELEASE(_name);
    CL_RELEASE(_info);

    _super::dealloc();
}


CL_CLASS_DEF_AS_SINGLETON(dCenter, dCocoa)

struct _Observer {
    dObject*                                observer;
    dSEL                                    allObjectsSel;
    CArrayT<std::pair<dObject*, dSEL> >*    pairs;
};


dCenter*
dCenter::init() {
    if (!_super::init()) return nil;
    _names = CL_NEW(_NamesDict);
    return this;
}

void
dCenter::dealloc() {
#if defined (_DEBUG)
    if (0 != _names->count())
        throw new ExExistSubscribers;
#endif

    CL_RELEASE(_names);
    _super::dealloc();
}

void                        
dCenter::subscribe(dObject* observer, dSEL sel, const dString* name, dObject* object) {
	CL_THROW_IF_TRUE(!observer || !sel);
    
    dArrayT<dData*>* vals = _names->objectForKeyNoException(name);
    if (!vals) {
        vals = CL_NEW(dArrayT<dData*>)->autorelease();
        _names->setObjectForKey(vals, name);
    }

    bool needsAdd = true;
    _Observer start = { 0 };
    start.observer = observer;
    _Observer* record = &start;
    for (dInteger i = 0, cnt = vals->count(); i < cnt; ++i) {
        _Observer* iter = (_Observer*)vals->objectAtIndex(i)->bytes();
        if (iter->observer == observer) {
            needsAdd = false;
            record = iter;
            break;
        }
    }

    if (object) {
        if (!record->pairs) record->pairs = new CArrayT<std::pair<dObject*, dSEL> >;
        record->pairs->addObject(std::make_pair(object, sel));
    }
    else record->allObjectsSel = sel;

    if (needsAdd)
        vals->addObject(dData::dataWithBytes(
            record, sizeof(struct _Observer)));
}

void
dCenter::unsubscribe(dObject* observer, const dString* name, dObject* object) {
    if (!observer) return;
    if (!object) {
        unsubscribe(observer, name);
        return;
    }

    dArrayT<dData*>* vals = _names->objectForKeyNoException(name);
    if (!vals) return;

    for (dInteger i = 0, cnt = vals->count(); i < cnt; ++i) {
        _Observer* iter = (_Observer*)vals->objectAtIndex(i)->bytes();
        if (iter->observer == observer) {
            dInteger pairsCnt = iter->pairs ? iter->pairs->count() : 0;
            for (dInteger j = pairsCnt - 1; j >= 0; --j) {
                std::pair<dObject*, dSEL> pair = iter->pairs->objectAtIndex(j);
                if (pair.first == object) {
                    iter->pairs->removeObjectAtIndex(j);
                    if (0 == iter->pairs->count()) {
                        SAFE_DELETE(iter->pairs);
                        if (!iter->allObjectsSel) vals->removeObjectAtIndex(i);
                        if (0 == vals->count()) _names->removeObjectForKey(name);
                    }
                }
            }
            break;
        }
    }
}

void
dCenter::unsubscribe(dObject* observer, const dString* name, bool needsLock) {
    if (!observer) return;

    dArrayT<dData*>* vals = _names->objectForKeyNoException(name);
    if (!vals) return;

    for (dInteger i = 0, cnt = vals->count(); i < cnt; ++i) {
        _Observer* iter = (_Observer*)vals->objectAtIndex(i)->bytes();
        if (iter->observer == observer) {
            SAFE_DELETE(iter->pairs);
            vals->removeObjectAtIndex(i);
            if (0 == vals->count()) _names->removeObjectForKey(name);
            break;
        }
    }
}

void
dCenter::unsubscribe(dObject* observer) {
    if (!observer) return;

    _NamesDict::Iterator iter(_names);
    while (iter.next())
        unsubscribe(observer, iter.key(), false);
}

void
dCenter::post(dNotification* ntf) const {
    if (!ntf || !ntf->name()) return;
    _LocalPool

    dArrayT<dObject*>* observers = CL_NEW(dArrayT<dObject*>)->autorelease();
    CArrayT<dSEL, dSEL> selectors;

    dArrayT<dData*>* vals = _names->objectForKeyNoException(ntf->name());
    if (!vals) return;

    for (dInteger i = 0, cnt = vals->count(); i < cnt; ++i) {
        _Observer* iter = (_Observer*)vals->objectAtIndex(i)->bytes();
        dSEL sel = nil;

        if (ntf->object() && iter->pairs) {
            for (dInteger j = 0, cnt = iter->pairs->count(); j < cnt; ++j) {
                std::pair<dObject*, dSEL> pair = iter->pairs->objectAtIndex(j);
                if (pair.first == ntf->object()) {
                    sel = pair.second;
                    break;
                }
            }
        } else {
            sel = iter->allObjectsSel;
        }

        if (sel) {
            observers->addObject(iter->observer);
            selectors.addObject(sel);
        }
    }

    try {
        for (dInteger i = 0, cnt = observers->count(); i < cnt; ++i)
            observers->objectAtIndex(i)->invokeSelector(selectors.objectAtIndex(i), ntf, nil);
    } catch (...) {
        // <todo>: need show message in log about exception
    }
}

void
dCenter::post(const dString* name, dObject* object, dDict* userInfo) const {
    dobj_ptr<dNotification> ntf(dNotification::alloc()->initWithParams(object, name, userInfo));
    post(ntf.get());
}