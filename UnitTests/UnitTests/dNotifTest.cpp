/*
** $Id: 2009/08/01, Taras Tovchenko $
** Unit Tests
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "TestClasses.h"


TEST_MSG_INFO(er0, "Notifications donsn\'t work\n")

static dInteger _sTestVal = 0;

class CFirstObserver : public dObject {
CL_CLASS_DECL(CFirstObserver, dObject)
public:
    CL_METHOD_DECL(onCatch)
};

class CSecondObserver : public dObject {
CL_CLASS_DECL(CSecondObserver, dObject)
public:
    CL_METHOD_DECL(onSome)
};

class CPoster : public dObject {
CL_CLASS_DECL(CPoster, dObject)
public:
    CL_NOTIF_DECL(kMessage)
    CL_NOTIF_DECL(kMessage2)
};

CL_NOTIF_DEF(CPoster, kMessage)
CL_NOTIF_DEF(CPoster, kMessage2)

CL_CLASS_DEF(CFirstObserver, dUnitTests)

CL_METHOD_DEF(CFirstObserver, onCatch) {
    _sTestVal += 3;
    return nil;
}

CL_CLASS_DEF(CSecondObserver, dUnitTests)

CL_METHOD_DEF(CSecondObserver, onSome) {
    _sTestVal += 2;
    return nil;
}

CL_CLASS_DEF(CPoster, dUnitTests)




void
NotifTest::run() {
    _LocalPool

    dobj_ptr<CFirstObserver> obs1(CL_NEW(CFirstObserver));
    dobj_ptr<CPoster> poster(CL_NEW(CPoster));

    dCenter::singleton()->subscribe(obs1.get(), dselector(onCatch), CPoster::kMessage, poster.get());
    dCenter::singleton()->post(CPoster::kMessage, poster.get());
    dCenter::singleton()->unsubscribe(obs1.get(), CPoster::kMessage, poster.get());
    if (3 != _sTestVal) testFail(er0);

    // post to unsubscribed
    dCenter::singleton()->post(CPoster::kMessage, poster.get());
    if (3 != _sTestVal) testFail(er0);

    // two subscribers
    dobj_ptr<CSecondObserver> obs2(CL_NEW(CSecondObserver));
    dCenter::singleton()->subscribe(obs1.get(), dselector(onCatch), CPoster::kMessage, poster.get());
    dCenter::singleton()->subscribe(obs2.get(), dselector(onSome), CPoster::kMessage, poster.get());

    dCenter::singleton()->post(CPoster::kMessage, poster.get());

    dCenter::singleton()->unsubscribe(obs1.get());
    dCenter::singleton()->unsubscribe(obs2.get());
    if (8 != _sTestVal) testFail(er0);

    dCenter::singleton()->subscribe(obs2.get(), dselector(onSome), CPoster::kMessage, nil);
    dCenter::singleton()->post(CPoster::kMessage, poster.get());
    dCenter::singleton()->unsubscribe(obs2.get(), CPoster::kMessage);
    if (10 != _sTestVal) testFail(er0);
}