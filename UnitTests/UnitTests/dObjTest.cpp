/*
** $Id: 2009/02/07, Taras Tovchenko $
** Unit Tests
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "TestClasses.h"


TEST_MSG_INFO(er0, "classByName from assembly don\'t work\n")
TEST_MSG_INFO(er1, "dInvocation don\'t work properly\n")
TEST_MSG_INFO(er2, "dObject casting don\'t work properly\n")
TEST_MSG_INFO(er3, "properties don\'t work properly\n")
TEST_MSG_INFO(er4, "dData don\'t work!\n")
TEST_MSG_INFO(er5, "dFileHandle don\'t work!\n")
TEST_MSG_INFO(er6, "dTreeNode don\'t work!\n")
TEST_MSG_INFO(er7, "dUuid don\'t work!\n")


static void     _checkFileReading(dFileHandle* file);


class CFirst : public dObject {
CL_CLASS_DECL(CFirst, dObject)
public:
};

CL_CLASS_DEF(CFirst, dUnitTests)

class CObjectCenter : public CFirst {
CL_CLASS_DECL_AS_SINGLETON(CObjectCenter, CFirst)
public:
    virtual void            func()  { }
};

CL_CLASS_DEF_AS_SINGLETON(CObjectCenter, dUnitTests)


class CTestAbstract : public dObject {
CL_ABSTRACT_DECL(CTestAbstract, dObject)
public:
    virtual void            func() = 0;

    CL_METHOD_DECL(onRender);
};

CL_ABSTRACT_DEF(CTestAbstract, dUnitTests)


class CObjTest : public CTestAbstract {
CL_CLASS_DECL(CObjTest, CTestAbstract)
public:
    virtual CObjTest*       init();

    virtual void            func() { }

    CL_METHOD_DECL(onSomethingDoit)
    CL_METHOD_DECL(onTimerFire)

    CL_PROP_DEF(dValue*, innerValue)

protected:
    CObjTest();
    virtual void            dealloc();
};


CObjTest::CObjTest()
: CL_PROP_INIT(innerValue) {
}

void
CObjTest::dealloc() {
    CL_PROP_RELEASE(innerValue);
    _super::dealloc();
}

class CKeyTest : public dObject {
CL_CLASS_DECL(CKeyTest, dObject)
public:
    static void                 initialize();
    virtual CKeyTest*           initWithStr(const char* str);

    static void                 deinitialize();

    virtual dInteger            hash() const;
    virtual bool                isEqual(const dObject* otherObject) const;

protected:
    CKeyTest();
    virtual void                dealloc();

    char*                       _str;
};

void
CKeyTest::initialize() {
    _tprintf(_T("Static Ctor call!\n"));
}

void
CKeyTest::deinitialize() {
    _tprintf(_T("Static Dtor call!\n"));
}

CL_CLASS_DEF_WITH_STATIC_CTORDTOR(CObjTest, dUnitTests, &CKeyTest::initialize, &CKeyTest::deinitialize);

CL_METHOD_DEF(CObjTest, onSomethingDoit) {
    dValue* first = CL_ARG(dValue, 0);
    dValue* second = CL_ARG(dValue, 1);

    if (234.56789 != first->doubleValue()) testFail(er1);
    if (7.45f != second->floatValue()) testFail(er1);

    _tprintf(_T("selector onSomethingDoit call!\n"));
    return nil;
}

CL_METHOD_DEF(CObjTest, onTimerFire) {
    return nil;
}

CL_METHOD_DEF(CTestAbstract, onRender) {
    dValue* firstArg = CL_ARG(dValue, 0);
    dValue* secondArg = CL_ARG(dValue, 1);

#if defined (_DEBUG)
    while (true) {
        try {
            CObjTest* dbgTst = CL_ARG(CObjTest, 0);
        } catch (ExSelectorWrongArgType* e) {
            SAFE_DELETE(e);
            break;
        }
    }
#endif

    if (8 != firstArg->intValue()) testFail(er1);
    if (7.45f != secondArg->floatValue()) testFail(er1);

    _tprintf(_T("selector onRender call!\n"));

    return dValue::valueWithInt(firstArg->intValue() + 1);
}

CObjTest*
CObjTest::init() {
    if (!_super::init()) return nil;
    return this;
}

CL_CLASS_DEF(CKeyTest, dUnitTests);

CKeyTest::CKeyTest()
: _str(nil) {
}

CKeyTest*
CKeyTest::initWithStr(const char* str) {
    if (!_super::init()) return nil;

    size_t len = strlen(str);
    _str = new char[len + 1];
    strcpy(_str, str);

    return this;
}

void
CKeyTest::dealloc() {
    SAFE_DELETE_ARRAY(_str);
    _super::dealloc();
}

dInteger
CKeyTest::hash() const {
    dInteger hash = 0;
    char* str = _str;
    for (;;) {
        dInteger a = *str++;
        if (0 == a) break;
        hash += (hash << 8) + a;
    }
    return hash;
}

bool
CKeyTest::isEqual(const dObject* otherObject) const {
    return !strcmp(_str, ((CKeyTest*)otherObject)->_str);
}



void
DObjectTest::run() {
    _LocalPool

    CObjTest* testObj = CL_NEW(CObjTest);
    testObj->autorelease();
    CObjTest* testObj2 = CL_NEW(CObjTest);

    dArray* objs = CL_NEW(dArray);
    dSet*   setObjs = CL_NEW(dSet);
    dDict*  mapObjs = CL_NEW(dDict);

    dArrayT<CObjTest*>* templArr = CL_NEW(dArrayT<CObjTest*>);
    dDictT<CObjTest*, CKeyTest*>* templDict = dDictT<CObjTest*, CKeyTest*>::alloc();
    templDict->init();
    dSetT<CObjTest*>* templSet = CL_NEW(dSetT<CObjTest*>);

    CKeyTest* key1 = CKeyTest::alloc()->initWithStr("key1")->autorelease();
    CKeyTest* key2 = CKeyTest::alloc()->initWithStr("key2");

    mapObjs->setObjectForKey(testObj, key1);
    templDict->setObjectForKey(testObj2, key2);

    objs->addObject(testObj);
    setObjs->addObject(testObj);
    setObjs->addObject(testObj2);
    templSet->addObject(testObj2);

    CL_RELEASE(testObj2);
    CL_RELEASE(key2);

    CL_RELEASE(templArr);
    CL_RELEASE(templDict);
    CL_RELEASE(templSet);
    CL_RELEASE(setObjs);
    CL_RELEASE(objs);
    CL_RELEASE(mapObjs);

    // ==============================================================================================

    const dClass* cls = dassembly(dUnitTests)->classByName("CObjTest");
    if (!cls) testFail(er0);

    CObjTest* cobjtest = dynamic_cast<CObjTest*>(cls->alloc());
    if (!cobjtest) testFail(er0);
    cobjtest->init()->autorelease();

    // ==============================================================================================

    dobj_ptr<CObjTest> targetObject = dobj_ptr<CObjTest>(CL_NEW(CObjTest));
    dobj_ptr<dInvocation> cmd = dobj_ptr<dInvocation>(dInvocation::alloc()->initWithParams(
        targetObject.get(),
        dselector(onRender),
        dValue::valueWithInt(8), dValue::valueWithFloat(7.45f), nil));

    cmd->invoke();
    if (9 != ((dValue*)cmd->returnValue())->intValue()) testFail(er1);

    targetObject->invokeSelector(
        dselector(onSomethingDoit),
        dValue::valueWithDouble(234.56789),
        dValue::valueWithFloat(7.45f),
        nil);

    if (targetObject->respondsToSelector(dselector(notExisting))) testFail(er1);
    if (!targetObject->respondsToSelector(dselector(onRender))) testFail(er1);

    // ==============================================================================================

    dobj_ptr<CObjTest> dob = dobj_ptr<CObjTest>(CL_NEW(CObjTest));

    if (!dobj_cast<CObjTest>(dob.get())) testFail(er2);

    // ==============================================================================================

    CObjectCenter::singleton()->func();
    // CObjectCenter::alloc(); not compile - this is good

    // ========== Timer test=========================================================================

//    dTimer::scheduledTimerWithTimeInterval(
//        10,
//        dob.get(),
//        dselector(onTimerFire),
//        nil,
//        true);

    // ==============================================================================================

    dobj_ptr<CObjTest> prpobj = dobj_ptr<CObjTest>(CL_NEW(CObjTest));

    prpobj->dprop(innerValue) = dValue::valueWithInt(3);
    int val2 = prpobj->dprop(innerValue)->intValue();
    if (3 != val2) testFail(er3);

    // ==============================================================================================

    dFloat buff[] = { 45.6f, 3.7f, 111.0f };
    dobj_ptr<dData> buffer = dobj_ptr<dData>(dData::alloc()->initWithBytes(buff, 3 * sizeof(dFloat)));
    if (45.6f != ((dFloat*)buffer->bytes())[0]) testFail(er4);
    if (3.7f != ((dFloat*)buffer->bytes())[1]) testFail(er4);
    if (111.0f != ((dFloat*)buffer->bytes())[2]) testFail(er4);

    dByte* dynBuff = new dByte[3 * sizeof(dFloat)];
    memcpy(dynBuff, buff, 3 * sizeof(dFloat));

    dobj_ptr<dData> buffer2 = dobj_ptr<dData>(dData::alloc()->initWithBytesNoCopy(dynBuff, 3 * sizeof(dFloat), true));
    if (45.6f != ((dFloat*)buffer2->bytes())[0]) testFail(er4);
    if (3.7f != ((dFloat*)buffer2->bytes())[1]) testFail(er4);
    if (111.0f != ((dFloat*)buffer2->bytes())[2]) testFail(er4);

    dobj_ptr<dData> cpy = dobj_ptr<dData>(dData::alloc()->initWithData(buffer2.get()));
    if (45.6f != ((dFloat*)cpy->bytes())[0]) testFail(er4);
    if (3.7f != ((dFloat*)cpy->bytes())[1]) testFail(er4);
    if (111.0f != ((dFloat*)cpy->bytes())[2]) testFail(er4);

    // =============================================================================================

    dFileHandle* file = dFileHandle::fileHandleForReadingAtPath(_ST("testfile.bin"));
    _checkFileReading(file);

    dFileHandle* file2 = dFileHandle::fileHandleForWritingAtPath(_ST("outfile.bin"));
    file->seekToFileOffset(0);
    file2->writeData(file->readDataToEndOfFile());
    file2->synchronizeFile();
    file2->closeFile();

    file2 = dFileHandle::fileHandleForUpdatingAtPath(_ST("outfile.bin"));
    _checkFileReading(file2);
    file2->truncateFileAtOffset(5);

    char bu[] = { 'A', 'B', 'C' };
    file2->writeData(dData::dataWithBytes(bu, 3));
    file2->synchronizeFile();
    file2->seekToFileOffset(0);

    // =============================================================================================

    dTreeNodeT<dValue*>* root = CL_NEW(dTreeNodeT<dValue*>);

    dTreeNodeT<dValue*>* c1 = CL_NEW(dTreeNodeT<dValue*>);
    c1->setValue(dValue::valueWithInt(22));
    root->addChild(c1);
    CL_RELEASE(c1);

    if (22 != root->childAtIndex(0)->value()->intValue()) testFail(er6);

    dTreeNodeT<dValue*>* c2 = CL_NEW(dTreeNodeT<dValue*>)->autorelease();
    root->addChild(c2);
    dTreeNodeT<dValue*>* l1 = dTreeNodeT<dValue*>::alloc()->initWithValue(
                                        dValue::valueWithInt(7))->autorelease();
    c2->addChild(l1);
    if (7 != root->childAtIndex(1)->childAtIndex(0)->value()->intValue()) testFail(er6);


    dTreeNode* root2 = CL_NEW(dTreeNode)->autorelease();
    dTreeNode* r1 = CL_NEW(dTreeNode)->autorelease();
    root2->addChild(r1);
    dTreeNode* root3 = CL_NEW(dTreeNode)->autorelease();
    root3->addChild(root2);

    root->detachChild(c2);
    CL_RELEASE(c2);

    CL_RELEASE(root);

    // =============================================================================================

//    dUuid* uid1 = dUuid::uniqueId();
//    dUuid* uid2 = dUuid::uniqueId();
//    if (uid1->isEqual(uid2)) testFail(er7);
}

static void
_checkFileReading(dFileHandle* file) {
    file->seekToFileOffset(5);
    dData* data = file->readDataToEndOfFile();
    file->seekToFileOffset(3);
    dData* data2 = file->readDataOfLength(2);

    dInteger cnt = data->length();
    if (9 != cnt) testFail(er5);
    char* str1 = new char[cnt + 1];
    memcpy(str1, data->bytes(), cnt);
    str1[cnt] = '\0';
    if (strcmp(str1, "Tovchenko")) testFail(er5);
    SAFE_DELETE_ARRAY(str1);

    dInteger cnt2 = data2->length();
    if (2 != cnt2) testFail(er5);
    str1 = new char[cnt2 + 1];
    memcpy(str1, data2->bytes(), cnt2);
    str1[cnt2] = '\0';
    if (strcmp(str1, "as")) testFail(er5);
    SAFE_DELETE_ARRAY(str1);
}