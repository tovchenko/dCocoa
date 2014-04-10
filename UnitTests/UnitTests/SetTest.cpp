/*
** $Id: 2009/02/07, Taras Tovchenko $
** Unit Tests
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "TestClasses.h"


TEST_MSG_INFO(er0, "Don\'t work count() in CSetT!\n");
TEST_MSG_INFO(er1, "Don\'t work iterator in CSetT!\n");
TEST_MSG_INFO(er2, "Don\'t work removeObject in CSetT!\n");
TEST_MSG_INFO(er3, "Don\'t work removeAllObjects in CSetT!\n");
TEST_MSG_INFO(er4, "Don\'t work addObjectsFromArray in CSetT!\n");
TEST_MSG_INFO(er5, "Don\'t work ctor in CSetT!\n");
TEST_MSG_INFO(er6, "Don\'t work isEqualToSet in CSetT!\n");
TEST_MSG_INFO(er7, "Don\'t work containsObject in CSetT!\n");
TEST_MSG_INFO(er8, "Don\'t work allObjects in CSetT!\n");
TEST_MSG_INFO(er9, "Don\'t work member in CSetT!\n");
TEST_MSG_INFO(er10, "Don\'t work anyObject in CSetT!\n");
TEST_MSG_INFO(er11, "Don\'t work createSetByAddingObject in CSetT!\n");
TEST_MSG_INFO(er12, "Don\'t work createSetByAddingObjectsFromArray in CSetT!\n");
TEST_MSG_INFO(er13, "Don\'t work createSetByAddingObjectsFromSet in CSetT!\n");
TEST_MSG_INFO(er14, "Don\'t work unionSet in CSetT!\n");
TEST_MSG_INFO(er15, "Don\'t work minusSet in CSetT!\n");
TEST_MSG_INFO(er16, "Don\'t work intersectSet in CSetT!\n");
TEST_MSG_INFO(er17, "Don\'t work setSet in CSetT!\n");
TEST_MSG_INFO(er18, "Don\'t work isSubsetOfSet in CSetT!\n");
TEST_MSG_INFO(er19, "Don\'t work intersectsSet in CSetT!\n");


template <typename TYPE>
class TestSetFuncs {
public:
    static dInteger     hashValueForKey(const TYPE* k) { return k->hashValue(); }
    static bool         isEqualKeys(const TYPE* k1, const TYPE* k2, void* context) { return !strcmp(k1->getValue(), k2->getValue()); }
};

void
SetTest::run() {
    CSetT<TestKey, TestSetFuncs> defSet;
	TestKey tk1; tk1.setValue("obj1");
    TestKey tk2; tk2.setValue("obj2");
    TestKey tk3; tk3.setValue("obj3");

    defSet.addObject(tk1);
    defSet.addObject(tk2);
    defSet.addObject(tk3);

    if (3 != defSet.count()) testFail(er0);

    CSetT<TestKey, TestSetFuncs>::Iterator iter1(&defSet);

    while (iter1.next()) {
        if (TestSetFuncs<TestKey>::isEqualKeys(&iter1.object(), &tk1, nil)) continue;
        if (TestSetFuncs<TestKey>::isEqualKeys(&iter1.object(), &tk2, nil)) continue;
        if (TestSetFuncs<TestKey>::isEqualKeys(&iter1.object(), &tk3, nil)) continue;
        testFail(er1);
    }

    defSet.removeObject(tk3);
    if (2 != defSet.count()) testFail(er2);

    defSet.removeAllObjects();
    if (0 != defSet.count()) testFail(er3);

	TestKey k5; k5.setValue("k5");
    TestKey k6; k6.setValue("k6");
    TestKey k7; k7.setValue("k7");
	TestKey k8; k8.setValue("k8");
	TestKey k9; k9.setValue("k9");
	TestKey s_1; s_1.setValue("s1");
	TestKey s_2; s_2.setValue("s2");
	TestKey s_3; s_3.setValue("s3");

    TestKey arrms[] = { k5, k6, k7, k8 };
    CArrayT<TestKey> arr(arrms, 4);
    
    TestKey tkss[] = { s_1, s_2, s_3 };
    CSetT<TestKey, TestSetFuncs> set(tkss, 3);

    set.addObjectsFromArray(&arr);

    if (7 != set.count()) testFail(er4);

    CSetT<TestKey, TestSetFuncs> set2(&arr);
    if (4 != set2.count()) testFail(er5);
    
    set.removeObject(s_1);
    set.removeObject(s_2);
    set.removeObject(s_3);

    if (!set2.isEqualToSet(&set)) testFail(er6);
    if (!set2.containsObject(k5)) testFail(er7);
    if (set2.containsObject(s_1)) testFail(er7);

    set.allObjects(&arr);

    if (arr.count() != set.count()) testFail(er8);
    for (dInteger i = 0, cnt = arr.count(); i < cnt; ++i)
        if (!set.containsObject(arr.objectAtIndex(i))) testFail(er8);

    try {
        set.member(k6);
    } catch (CSetT<TestKey, TestSetFuncs>::ExSetObjectNotFound* e) {
        testFail(er9);
        SAFE_DELETE(e);
    }
    try {
        set.member(k9);
        testFail(er9);
    } catch (CSetT<TestKey, TestSetFuncs>::ExSetObjectNotFound* e) {
        SAFE_DELETE(e);
    }


    CSetT<TestKey, TestSetFuncs> set3(set2);

    if (!set3.isEqualToSet(&set2)) testFail(er5);

    TestKey* cSet = new TestKey[3];
    cSet[0].setValue("b1");
    cSet[1].setValue("b2");
    cSet[2].setValue("b3");

    CSetT<TestKey, TestSetFuncs> setCSet(cSet, 3);
    SAFE_DELETE_ARRAY(cSet);

    if (3 != setCSet.count()) testFail(er5);
	
	TestKey b1; b1.setValue("b1");
    TestKey b2; b2.setValue("b2");
    TestKey b3; b3.setValue("b3");
	TestKey b4; b4.setValue("b4");

    if (!setCSet.containsObject(b1)) testFail(er5);
    if (!setCSet.containsObject(b2)) testFail(er5);
    if (!setCSet.containsObject(b3)) testFail(er5);
    if (setCSet.containsObject(b4)) testFail(er5);

    try {
        TestKey anyKey = setCSet.anyObject();
        if (!setCSet.containsObject(anyKey)) testFail(er10);
    } catch (CSetT<TestKey, TestSetFuncs>::ExSetObjectNotFound* e) {
        testFail(er10);
        SAFE_DELETE(e);
    }

    setCSet.removeAllObjects();

    try {
        setCSet.anyObject();
        testFail(er10);
    } catch (CSetT<TestKey, TestSetFuncs>::ExSetObjectNotFound* e) {
        SAFE_DELETE(e);
    }


	TestKey newobj; newobj.setValue("newObj");
    CSetT<TestKey, TestSetFuncs>* newly = set3.newSetByAddingObject(newobj);
    if (!newly->containsObject(newobj)) testFail(er11);
    SAFE_DELETE(newly);

	TestKey k1; k1.setValue("kk1");
	TestKey k2; k2.setValue("kk2");
	TestKey k3; k3.setValue("kk3");
	TestKey k4; k4.setValue("kk4");
	TestKey l1; l1.setValue("l1");
	TestKey l2; l2.setValue("l2");
	TestKey l3; l3.setValue("l3");
	
    TestKey keysms[] = { k1, k2, k3 };
	CArrayT<TestKey> keys(keysms, 3);
    CSetT<TestKey, TestSetFuncs>* newly2 = set3.newSetByAddingObjectsFromArray(&keys);
    if (!newly2->containsObject(k1)) testFail(er12);
    if (!newly2->containsObject(k2)) testFail(er12);
    if (newly2->containsObject(k4)) testFail(er12);
    if (!newly2->containsObject(k3)) testFail(er12);
    if (!newly2->containsObject(set3.anyObject())) testFail(er12);
    SAFE_DELETE(newly2);

    TestKey oness[] = { l1, l2 };
    CSetT<TestKey, TestSetFuncs> oneSet(oness, 2);
    CSetT<TestKey, TestSetFuncs> secondSet(&l3, 1);
    secondSet.unionSet(&oneSet);

    if (3 != secondSet.count()) testFail(er14);
    if (!secondSet.containsObject(l2)) testFail(er14);
    if (!secondSet.containsObject(l3)) testFail(er14);
    if (!secondSet.containsObject(l1)) testFail(er14);


    CSetT<TestKey, TestSetFuncs>* thirdSet = secondSet.newSetByAddingObjectsFromSet(&set3);
    if (!thirdSet->containsObject(l2)) testFail(er13);
    if (!thirdSet->containsObject(set3.anyObject())) testFail(er13);
    if (!thirdSet->containsObject(l3)) testFail(er13);
    if (!thirdSet->containsObject(l1)) testFail(er13);
    SAFE_DELETE(thirdSet);


	TestKey one; one.setValue("one");
	TestKey two; two.setValue("two");
	TestKey three; three.setValue("three");
	TestKey four; four.setValue("four");
	TestKey fifth; fifth.setValue("fifth");
	TestKey six; six.setValue("six");
	
    TestKey rss[] = { one, two, three, four };
    CSetT<TestKey, TestSetFuncs> rSet(rss, 4);
    TestKey r2ss[] = { one, three, four };
    CSetT<TestKey, TestSetFuncs> rSet2(r2ss, 3);
    rSet.minusSet(&rSet2);

    if (1 != rSet.count()) testFail(er15);
    TestKey tTwo; tTwo.setValue("two");
    if (!TestSetFuncs<TestKey>::isEqualKeys(&rSet.anyObject(), &tTwo, nil)) testFail(er15);


    TestKey iss[] = { one, fifth, six, four };
    CSetT<TestKey, TestSetFuncs> iSet(rss, 4);
    CSetT<TestKey, TestSetFuncs> iSet2(iss, 4);
    iSet.intersectSet(&iSet2);

    CSetT<TestKey, TestSetFuncs>::Iterator iSetIter(&iSet);
    TestKey tOne; tOne.setValue("one");
    TestKey tFour; tFour.setValue("four");

    while (iSetIter.next()) {
        if (TestSetFuncs<TestKey>::isEqualKeys(&iSetIter.object(), &tOne, nil)) continue;
        if (TestSetFuncs<TestKey>::isEqualKeys(&iSetIter.object(), &tFour, nil)) continue;
        testFail(er16);
    }

	TestKey boo1; boo1.setValue("boo1");
	TestKey boo2; boo2.setValue("boo2");
	TestKey boo3; boo3.setValue("boo3");
	TestKey boo4; boo4.setValue("boo4");
	TestKey capacity; capacity.setValue("capacity");
	TestKey other; other.setValue("other");
	TestKey bla; bla.setValue("bla-bla");
	TestKey yahoo; yahoo.setValue("yahoo");
	
    TestKey pss[] = { boo1, boo2, boo3 };
    CSetT<TestKey, TestSetFuncs> pSet(pss, 3);
    iSet.setSet(&pSet);

    if (iSet.containsObject(one)) testFail(er17);
    if (iSet.containsObject(four)) testFail(er17);
    if (!iSet.containsObject(boo1)) testFail(er17);
    if (!iSet.containsObject(boo3)) testFail(er17);

    CSetT<TestKey, TestSetFuncs> capacitySet(10);
    capacitySet.addObject(capacity);
    if (1 != capacitySet.count()) testFail(er5);
    if (!capacitySet.containsObject(capacity)) testFail(er5);


    iSet.addObject(other);
    TestKey subss[] = { boo1, boo3 };
    CSetT<TestKey, TestSetFuncs> subSet(subss, 2);

    if (!subSet.isSubsetOfSet(&iSet)) testFail(er18);
    if (subSet.isSubsetOfSet(&capacitySet)) testFail(er18);


    TestKey i1ss[] = { yahoo, bla, boo3 };
    CSetT<TestKey, TestSetFuncs> interSet1(i1ss, 3);
    TestKey i2ss[] = { yahoo, bla, boo4 };
    CSetT<TestKey, TestSetFuncs> interSet2(i2ss, 3);

    if (!subSet.intersectsSet(&interSet1)) testFail(er19);
    if (subSet.intersectsSet(&interSet2)) testFail(er19);
}