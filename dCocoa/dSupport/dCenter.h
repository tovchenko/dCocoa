/*
** $Id: 2009/07/30, Taras Tovchenko $
** Notification - container which keep object, notif-string, additional info
** Notification center manage subscribed objects, notif-messages
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DNOTIFICATIONCENTER_H
#define _COCOA_DNOTIFICATIONCENTER_H


#define CL_NOTIF_DECL(notifName)            static const CL::dString*   notifName;
#define CL_NOTIF_DEF(clsName, notifName)    CL::dStaticString __sNotif_##clsName##notifName(_T(#notifName));    \
                                            const CL::dString* clsName::notifName = &__sNotif_##clsName##notifName;

BEGIN_NAMESPACE_CL
class dString;
class dDict;
class dData;
template <typename T> class dArrayT;
template <typename T1, typename T2> class dDictT;
typedef dDictT<dArrayT<dData*>*, const dString*>    _NamesDict;
class dCriticalSection;


class DLL_PUBLIC dNotification : public dObject {
CL_CLASS_DECL(dNotification, dObject)
public:
    static dNotification*       notificationWithParams(dObject* object, const dString* name, dDict* userInfo);
    virtual dNotification*      initWithParams(dObject* object, const dString* name, dDict* userInfo);

    dObject*                    object() const { return _object; }
    const dString*              name() const { return _name; }
    dDict*                      userInfo() const { return _info; }

protected:
    dNotification();
    virtual void                dealloc();

    dObject*                    _object;
    dString*                    _name;
    dDict*                      _info;
};


class DLL_PUBLIC dCenter : public dObject {
CL_CLASS_DECL_AS_SINGLETON(dCenter, dObject)
public:
    // exceptions
    class ExExistSubscribers : public CL::CException { };
    // subscribe / unsubscribe
    void                        subscribe(dObject* observer, dSEL sel, const dString* name, dObject* object);
    void                        unsubscribe(dObject* observer, const dString* name, dObject* object);
    void                        unsubscribe(dObject* observer, const dString* name, bool needsLock = true);
    void                        unsubscribe(dObject* observer);

    void                        post(dNotification* ntf) const;
    void                        post(const dString* name, dObject* object, dDict* userInfo = nil) const;

protected:
    virtual dCenter*            init();
    virtual void                dealloc();

    _NamesDict*                 _names;
};

END_NAMESPACE_CL

#endif // _COCOA_DNOTIFICATIONCENTER_H