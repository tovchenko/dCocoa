/*
** $Id: 2009/02/07, Taras Tovchenko $
** Unit Tests
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "TestClasses.h"


TEST_MSG_INFO(er0, "Don\'t work setObjectForKey or objectForKey method in CDictT!\n");
TEST_MSG_INFO(er1, "Don\'t work removeObjectForKey method in CDictT!\n");
TEST_MSG_INFO(er2, "Don\'t work iterator functionality in CDictT!\n");
TEST_MSG_INFO(er3, "Don\'t work CDictT cArrays ctor in CDictT!\n");
TEST_MSG_INFO(er4, "Don\'t work copy-ctor in CDictT!\n");
TEST_MSG_INFO(er5, "Don\'t work isEqualToDictionary in CDictT!\n");
TEST_MSG_INFO(er6, "Don\'t work allKeys in CDictT!\n");
TEST_MSG_INFO(er7, "Don\'t work allKeysForObject in CDictT!\n");
TEST_MSG_INFO(er8, "Don\'t work allValues in CDictT!\n");
TEST_MSG_INFO(er9, "Don\'t work getObjectsAndKeys in CDictT!\n");
TEST_MSG_INFO(er10, "Don\'t work objectsForKeysNotFoundMarker in CDictT!\n");



template <typename OBJ_TYPE, typename KEY_TYPE>
class isHashEqual {
public:
    static dInteger     hashValueForKey(const KEY_TYPE* k) { return k->hashValue(); }
    static bool         isEqualKeys(const KEY_TYPE* k1, const KEY_TYPE* k2, void* context) { return !strcmp(k1->getValue(), k2->getValue()); }
    static bool         isEqualObjects(const OBJ_TYPE* o1, const OBJ_TYPE* o2, void* context) { return o1->getValue() == o2->getValue(); }
};


void
DictionaryTest::run() {
    TestValue* obj = new TestValue; obj->setValue(23);
    TestKey* key = new TestKey; key->setValue("key0");

    TestValue obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9;
	obj1.setValue(45);
	obj2.setValue(46);
	obj3.setValue(47);
	obj4.setValue(48);
	obj5.setValue(49);
	obj6.setValue(50);
	obj7.setValue(51);
	obj8.setValue(52);
	obj9.setValue(53);
	
    TestKey key1, key2, key3, key4, key5, key6, key7, key8, key9;
	key1.setValue("key1");
	key2.setValue("key2");
	key3.setValue("key3");
	key4.setValue("key4");
	key5.setValue("key5");
	key6.setValue("key6");
	key7.setValue("key7");
	key8.setValue("key8");
	key9.setValue("key9");
	
    TestValue tvals[] = { obj2, obj3, obj4, obj5, obj6 };
    TestKey tkeys[] = { key2, key3, key4, key5, key6 };
    CDictT<TestValue, TestKey, isHashEqual> myMap(tvals, tkeys, 5);

	TestKey key_4; key_4.setValue("key4");
    TestValue tst = myMap.objectForKey(key_4);
    if (48 != tst.getValue()) testFail(er0);
    TestValue tst1 = myMap.objectForKey(key2);
    if (obj2.getValue() != tst1.getValue()) testFail(er0);
    TestValue tst2 = myMap.objectForKey(key6);
    if (obj6.getValue() != tst2.getValue()) testFail(er0);

    CDictT<TestValue, TestKey, isHashEqual>::Iterator it1(&myMap);

    dInteger itCnt = 0;
    while (it1.next()) {
        switch (it1.object().getValue()) {
            case 46:
            case 47:
            case 48:
            case 49:
            case 50:
                break;
            default:
                testFail(er2);
        }
        ++itCnt;
    }
    if (5 != itCnt) testFail(er2);


    CDictT<TestValue, TestKey, isHashEqual> mySecMap;

    mySecMap.setObjectForKey(obj7, key7);
    mySecMap.setObjectForKey(obj8, key8);
    mySecMap.setObjectForKey(obj9, key8); // same key test

    myMap.addEntriesFromDictionary(&mySecMap);

    itCnt = 0;
    it1.reset();
    int a;
    while (it1.next()) {
        switch (a = it1.object().getValue()) {
            case 46:
            case 47:
            case 48:
            case 49:
            case 50:
            case 51:
            case 53:
                break;
            default:
                testFail(er2);
        }
        ++itCnt;
    }
    if (7 != itCnt) testFail(er2);

    CDictT<TestValue, TestKey, isHashEqual>*  dict = new CDictT<TestValue, TestKey, isHashEqual>;

    dict->setObjectForKey(*obj, *key);
    dict->setObjectForKey(obj1, key1);

    TestValue& testVal = dict->objectForKey(*key);
    if (23 != testVal.getValue()) testFail(er0);
    TestValue& testVal1 = dict->objectForKey(key1);
    if (45 != testVal1.getValue()) testFail(er0);


    CDictT<TestValue, TestKey, isHashEqual>::Iterator iter(dict);

    dInteger lcount = 0;
    while (iter.next()) {
        iter.object();
        ++lcount;
    }
    if (2 != lcount) testFail(er2);


    dict->removeObjectForKey(*key);

    TestValue& testVal2 = dict->objectForKey(key1);
    if (45 != testVal2.getValue()) testFail(er1);
    try {
      //  TestValue& testVal3 = dict->objectForKey(*key);
    } catch (CDictT<TestValue, TestKey, isHashEqual>::ExDictObjectNotFound* e) {
        if (strcmp(e->notFoundKey().getValue(), key->getValue())) testFail(er1);
        SAFE_DELETE(e);
    }

    SAFE_DELETE(dict);
    SAFE_DELETE(key);
    SAFE_DELETE(obj);

    const dInteger kLocalCount = 4;

    TestValue* cValues = new TestValue[kLocalCount];
    cValues[0].setValue(0);
    cValues[1].setValue(1);
    cValues[2].setValue(2);
    cValues[3].setValue(3);

    TestKey* cKeys = new TestKey[kLocalCount];
    cKeys[0].setValue("some0");
    cKeys[1].setValue("some1");
    cKeys[2].setValue("some2");
    cKeys[3].setValue("some3");

    CDictT<TestValue, TestKey, isHashEqual> cDict(cValues, cKeys, kLocalCount);
    if (kLocalCount != cDict.count()) testFail(er3);

    SAFE_DELETE_ARRAY(cValues);
    SAFE_DELETE_ARRAY(cKeys);

	TestKey some_0, some_2, some_3;
	some_0.setValue("some0");
	some_2.setValue("some2");
	some_3.setValue("some3");
	
    TestValue tt0 = cDict.objectForKey(some_0);
    if (0 != tt0.getValue()) testFail(er3);
    TestValue tt1 = cDict.objectForKey(some_3);
    if (3 != tt1.getValue()) testFail(er3);


    CDictT<TestValue, TestKey, isHashEqual> dictWdict(cDict);

    if (cDict.count() != dictWdict.count()) testFail(er4);
    TestValue tt2 = cDict.objectForKey(some_0);
    if (0 != tt2.getValue()) testFail(er4);
    TestValue tt3 = cDict.objectForKey(some_3);
    if (3 != tt3.getValue()) testFail(er4);

    if (!cDict.isEqualToDictionary(&dictWdict)) testFail(er5);
	TestValue vvv; vvv.setValue(1234);
    dictWdict.setObjectForKey(vvv, some_2);
    if (cDict.isEqualToDictionary(&dictWdict)) testFail(er5);

	TestValue vv; vv.setValue(145);
	TestKey kk; kk.setValue("yov!");
    dictWdict.setObjectForKey(vv, kk);


    CArrayT<TestKey> keys;
    dictWdict.allKeys(&keys);

    if (dictWdict.count() != keys.count()) testFail(er6);

    try {
        for (dInteger i = 0, cnt = keys.count(); i < cnt; ++i)
            dictWdict.objectForKey(keys.objectAtIndex(i));
    } catch (CDictT<TestValue, TestKey, isHashEqual>::ExDictObjectNotFound* e) {
        testFail(er6);
        SAFE_DELETE(e);
    }


	TestValue one; one.setValue(1);
	TestKey second; second.setValue("secondKey");
	TestKey third; third.setValue("thirdKey");
	
    dictWdict.setObjectForKey(one, second);
    dictWdict.setObjectForKey(one, third);
    CArrayT<TestKey> keysAtObj1;
    dictWdict.allKeysForObject(&keysAtObj1, one);

    if (3 != keysAtObj1.count()) testFail(er7);


    CArrayT<TestValue> valArr;
    dictWdict.allValues(&valArr);

    if (dictWdict.count() != valArr.count()) testFail(er8);

    dInteger sameObjCnt = 0;
    dInteger oneObjCnt = 0;
    for (dInteger i = 0, cnt = valArr.count(); i < cnt; ++i) {
        TestValue tVal; tVal.setValue(1);
        if (isHashEqual<TestValue, TestKey>::isEqualObjects(&tVal, &valArr.objectAtIndex(i), nil)) {
            ++sameObjCnt;
            continue;
        }
        TestValue tVal2; tVal2.setValue(3);
        if (isHashEqual<TestValue, TestKey>::isEqualObjects(&tVal2, &valArr.objectAtIndex(i), nil))
            ++oneObjCnt;
    }
    if (3 != sameObjCnt) testFail(er8);
    if (1 != oneObjCnt) testFail(er8);


	TestValue six, eigth;
	six.setValue(6);
	eigth.setValue(8);
	
	TestKey m6, m7, m8;
	m6.setValue("mk6");
	m7.setValue("mk7");
	m8.setValue("mk8");
    
    TestValue tvms[] = { six, six, eigth };
	TestKey tkms[] = { m6, m7, m8 };
    CDictT<TestValue, TestKey, isHashEqual> minDict(tvms, tkms, 3);

    TestValue* valAr = (TestValue*)operator new (sizeof(TestValue) * 3);
    TestKey* keyAr = (TestKey*)operator new (sizeof(TestKey) * 3);
    minDict.getObjectsAndKeys(valAr, keyAr);

    for (dInteger i = 0, cnt = minDict.count(); i < cnt; ++i) {
        try {
            if (!isHashEqual<TestValue, TestKey>::isEqualObjects(&valAr[i], &minDict.objectForKey(keyAr[i]), nil))
                testFail(er9);
        } catch (CDictT<TestValue, TestKey, isHashEqual>::ExDictObjectNotFound* e) {
            testFail(er9);
            SAFE_DELETE(e);
        }
    }

//    for (dInteger i = 0, cnt = minDict.count(); i < cnt; ++i) {
//        valAr[i].~TestValue();
//        keyAr[i].~TestKey();
//    }
    operator delete (valAr);
    operator delete (keyAr);


    CArrayT<TestValue>  values1;
	TestKey notExist; notExist.setValue("notExist");
	
    TestKey tms[] = { m7, notExist, m8 };
    CArrayT<TestKey>    keys1(tms, 3);
	
	TestValue v2000; v2000.setValue(2000);
    minDict.objectsForKeysNotFoundMarker(&values1, &keys1, v2000);

    if (6 != values1.objectAtIndex(0).getValue()) testFail(er10);
    if (2000 != values1.objectAtIndex(1).getValue()) testFail(er10);
    if (8 != values1.lastObject().getValue()) testFail(er10);
}