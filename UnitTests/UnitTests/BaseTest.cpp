/*
** $Id: 2009/02/07, Taras Tovchenko $
** Unit Tests
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "TestClasses.h"

#include <cstdlib>
#include <ctime>
#include <memory>


TEST_MSG_INFO(er0, "Don\'t work objectAtIndex method in CArrayT!");
TEST_MSG_INFO(er1, "Don\'t work indexOfObject method in CArrayT!");
TEST_MSG_INFO(er2, "Don\'t work getObjects method in CArrayT!");
TEST_MSG_INFO(er3, "Don\'t work insertObjectAtIndex method in CArrayT!");
TEST_MSG_INFO(er4, "Don\'t work removeObject method in CArrayT!");
TEST_MSG_INFO(er5, "Don\'t work removeObjectInRange method in CArrayT!");
TEST_MSG_INFO(er6, "Don\'t work removeObjectsInArray method in CArrayT!");
TEST_MSG_INFO(er7, "Don\'t work replaceObjectAtIndexWithObject method in CArrayT!");
TEST_MSG_INFO(er8, "Don\'t work removeObjectsFromIndices method in CArrayT!");
TEST_MSG_INFO(er9, "Don\'t work replaceObjectsInRangeWithObjectsFromArrayRange method in CArrayT!");
TEST_MSG_INFO(er10, "Don\'t work sort method in CArrayT!");
TEST_MSG_INFO(er11, "Don\'t work isEqualToArray method in CArrayT!");
TEST_MSG_INFO(er12, "Don\'t work firstObjectCommonWithArray method in CArrayT!");
TEST_MSG_INFO(er13, "Don\'t work CArrayT!");
TEST_MSG_INFO(er14, "Don\'t work setObjectsToRange in CArrayT!");
TEST_MSG_INFO(er15, "Don\'t work CTreeNodeT!");


class TestObject {
public:
    TestObject(dFloat f, dInteger i) { _fpointVal = new dFloat(f); _intVal = new dInteger(i); }
    TestObject(const TestObject& rf) {
        _fpointVal = new dFloat(*rf._fpointVal);
        _intVal = new dInteger(*rf._intVal);
    }

    ~TestObject() { SAFE_DELETE(_fpointVal); SAFE_DELETE(_intVal); }

    TestObject& operator=(const TestObject& rf) {
        if (this == &rf) return *this;
        *_fpointVal = *rf._fpointVal;
        *_intVal = *rf._intVal;

        return *this;
    }

    dFloat*     _fpointVal;
    dInteger*   _intVal;
};

static dComparisonResult   integerComparator(const void* obj1, const void* obj2, void* context);

void
BaseTest::run() {

    dInteger a0 = 2, a1 = 3, a2 = 34, a3 = 8, a4 = 5, a5 = 34, a6 = 6, a7 = 10, some = 29900;
    dInteger ms[] = { a0, a1, a2, a3, a4, a5, a6, a7 };
    CArrayT<dInteger>* arr = new CArrayT<dInteger>(ms, 8);

    if (arr->objectAtIndex(3) != 8)
        testFail(er0);
    if (arr->objectAtIndex(0) != 2)
        testFail(er0);
    if (arr->objectAtIndex(7) != 10)
        testFail(er0);

    // =========================================================================
    dFloat cArray[] = { 5.6f, 3.5f, 2.23f, 10.1f, 3.16f, 9.0f };
    CArrayT<dFloat> arr2(cArray, 6);

    if (arr2.objectAtIndex(3) != 10.1f)
        testFail(er0);
    if (arr2.objectAtIndex(0) != 5.6f)
        testFail(er0);
    if (arr2.objectAtIndex(5) != 9.0f)
        testFail(er0);

    // =========================================================================
    CArrayT<dInteger> arr3(arr);
    SAFE_DELETE(arr);

    if (arr3.objectAtIndex(3) != 8)
        testFail(er0);
    if (arr3.objectAtIndex(0) != 2)
        testFail(er0);
    if (arr3.objectAtIndex(7) != 10)
        testFail(er0);

    // =========================================================================
    if (4 != arr3.indexOfObject(a4))
        testFail(er1);
    if (0 != arr3.indexOfObject(a0))
        testFail(er1);
    if (7 != arr3.indexOfObject(a7))
        testFail(er1);
    if (kNotFound != arr3.indexOfObject(some))
        testFail(er1);

    if (4 != arr3.indexOfObjectInRange(a4, dMakeRange(3, 2)))
        testFail(er1);
    if (0 != arr3.indexOfObjectInRange(a0, dMakeRange(0, 8)))
        testFail(er1);
    if (kNotFound != arr3.indexOfObjectInRange(a7, dMakeRange(2, 3)))
        testFail(er1);
    if (kNotFound != arr3.indexOfObjectInRange(some, dMakeRange(0, 0)))
        testFail(er1);

    // =========================================================================
    dInteger cArr[8];
    arr3.getObjects(cArr);

    if (8 != cArr[3]) testFail(er2);
    if (2 != cArr[0]) testFail(er2);
    if (10 != cArr[7]) testFail(er2);

    dInteger cSubArr[4];
    arr3.getObjectsInRange(cSubArr, dMakeRange(3, 4));

    if (8 != cSubArr[0]) testFail(er2);
    if (5 != cSubArr[1]) testFail(er2);
    if (34 != cSubArr[2]) testFail(er2);
    if (6 != cSubArr[3]) testFail(er2);

    // =========================================================================
    CArrayT<dInteger>* subarray = arr3.newSubarrayWithRange(dMakeRange(2, 2));

    if (34 != subarray->objectAtIndex(0)) testFail(er0);
    if (8 != subarray->objectAtIndex(1)) testFail(er0);
    if (2 != subarray->count()) testFail(er0);

    SAFE_DELETE(subarray);

    // =========================================================================
    // Adding items
    // Print result array
    dInteger i3 = 99, i0 = 97, ilast = 89, ad0 = 66, ad1 = 77, ad2 = 88;
    arr3.insertObjectAtIndex(i3, 3);
    if (99 != arr3.objectAtIndex(3)) testFail(er3);
    arr3.insertObjectAtIndex(i0, 0);
    if (97 != arr3.objectAtIndex(0)) testFail(er3);
    arr3.insertObjectAtIndex(ilast, arr3.count());
    if (89 != arr3.objectAtIndex(arr3.count() - 1)) testFail(er3);

    arr3.addObject(ad0);
    arr3.addObject(ad1);
    arr3.addObject(ad2);

    if (66 != arr3.objectAtIndex(arr3.count() - 3)) testFail(er3);
    if (77 != arr3.objectAtIndex(arr3.count() - 2)) testFail(er3);
    if (88 != arr3.objectAtIndex(arr3.count() - 1)) testFail(er3);

    // =========================================================================
    // Print result array
    dInteger j0 = 20, j1 = 30, j2 = 40, j3 = 50;
    dInteger ims[] = { j0, j1, j2, j3 };
    CArrayT<dInteger> tmpArr(ims, 4);

    arr3.addObjectsFromArray(&tmpArr);

    // =========================================================================
    CArrayT<dInteger>* allocArr = tmpArr.newArrayByAddingObject(j3);
    CArrayT<dInteger>* alloc2 = tmpArr.newArrayByAddingObjectsFromArray(allocArr);

    alloc2->removeObjectAtIndex(4);
    alloc2->removeObjectAtIndex(0);
    alloc2->removeLastObject();

    alloc2->removeObject(j2);
    if (30 != alloc2->objectAtIndex(0)) testFail(er4);
    if (50 != alloc2->lastObject()) testFail(er4);

    alloc2->addObject(j2);
    alloc2->addObject(j1);
    alloc2->addObject(j1);
    alloc2->addObject(j3);
    alloc2->addObject(j1);

    alloc2->removeObjectInRange(j1, dMakeRange(2, 4));

    if (30 != alloc2->objectAtIndex(0)) testFail(er5);
    if (50 != alloc2->objectAtIndex(1)) testFail(er5);
    if (50 != alloc2->objectAtIndex(2)) testFail(er5);
    if (40 != alloc2->objectAtIndex(3)) testFail(er5);
    if (30 != alloc2->lastObject()) testFail(er5);

    // alloc2->removeObjectsInRange(dMakeRange(3, 1));
    // alloc2->removeAllObjects();
    // alloc2->removeAllObjects();

    

    // =========================================================================
    CArrayT<dInteger>* newArr = alloc2->newArrayByAddingObjectsFromArray(allocArr);
    dInteger nms[] = { j2, j1, j2 };
    CArrayT<dInteger> delArr(nms, 3);

    newArr->removeObjectsInArray(&delArr);

    if (50 != newArr->objectAtIndex(0)) testFail(er6);
    if (50 != newArr->objectAtIndex(1)) testFail(er6);
    if (20 != newArr->objectAtIndex(3)) testFail(er6);
    if (50 != newArr->lastObject()) testFail(er6);

    newArr->replaceObjectAtIndexWithObject(2, i3);
    if (99 != newArr->objectAtIndex(2)) testFail(er7);

    newArr->addObjectsFromArray(&delArr);

    dInteger indices[] = { 2, 5, 1, 1, 1, 1 };
    newArr->removeObjectsFromIndices(indices, 6);

    if (50 != newArr->objectAtIndex(0)) testFail(er8);
    if (20 != newArr->objectAtIndex(1)) testFail(er8);
    if (50 != newArr->objectAtIndex(2)) testFail(er8);
    if (40 != newArr->lastObject()) testFail(er8);

    newArr->addObjectsFromArray(alloc2);
    dInteger s0 = 3, s1 = 4, s2 = 5, s3 = 6;
    dInteger sms[] = { s0, s1, s2, s3 };
    CArrayT<dInteger> simpleArr(sms, 4);

    newArr->replaceObjectsInRangeWithObjectsFromArrayRange(dMakeRange(3, 3), &simpleArr, dMakeRange(1, 3));
    newArr->replaceObjectsInRangeWithObjectsFromArrayRange(dMakeRange(0, 2), &simpleArr, dMakeRange(2, 2));
    newArr->replaceObjectsInRangeWithObjectsFromArrayRange(dMakeRange(8, 1), &simpleArr, dMakeRange(0, 1));

    if (5 != newArr->objectAtIndex(0)) testFail(er9);
    if (6 != newArr->objectAtIndex(1)) testFail(er9);
    if (3 != newArr->objectAtIndex(8)) testFail(er9);
    if (30 != newArr->lastObject()) testFail(er9);

    dInteger m0 = 22, m1 = 23, m2 = 24, m3 = 25, m4 = 26;
    dInteger mms[] = { m0, m1, m2, m3, m4 };
    CArrayT<dInteger> lowArr(mms, 5);

    newArr->replaceObjectsInRangeWithObjectsFromArrayRange(dMakeRange(2, 5), &lowArr, dMakeRange(0, 3));

    if (5 != newArr->objectAtIndex(0)) testFail(er9);
    if (23 != newArr->objectAtIndex(3)) testFail(er9);
    if (50 != newArr->objectAtIndex(5)) testFail(er9);
    if (30 != newArr->lastObject()) testFail(er9);

    simpleArr.replaceObjectsInRangeWithObjectsFromArrayRange(dMakeRange(1, 2), newArr, dMakeRange(2, 4));
    simpleArr.replaceObjectsInRangeWithObjectsFromArrayRange(dMakeRange(0, 1), newArr, dMakeRange(0, 2));
    simpleArr.replaceObjectsInRangeWithObjectsFromArrayRange(dMakeRange(6, 1), newArr, dMakeRange(4, 3));

    if (5 != simpleArr.objectAtIndex(0)) testFail(er9);
    if (23 != simpleArr.objectAtIndex(3)) testFail(er9);
    if (50 != simpleArr.objectAtIndex(7)) testFail(er9);
    if (3 != simpleArr.lastObject()) testFail(er9);

    simpleArr.replaceObjectsInRangeWithObjectsFromArray(dMakeRange(0, simpleArr.count() - 1), newArr);

    for (dInteger i = 0, cnt = simpleArr.count(); i < cnt; ++i)
        _tprintf(_T("item #%d = %d\n"), i, simpleArr.objectAtIndex(i));
/*
    simpleArr.setComparator(integerComparator, nil, true);
    simpleArr.sort();

    if (3 != simpleArr.objectAtIndex(0)) testFail(er10);
    if (5 != simpleArr.objectAtIndex(1)) testFail(er10);
    if (6 != simpleArr.objectAtIndex(2)) testFail(er10);
    if (22 != simpleArr.objectAtIndex(3)) testFail(er10);
    if (23 != simpleArr.objectAtIndex(4)) testFail(er10);
    if (40 != simpleArr.objectAtIndex(5)) testFail(er10);
    if (50 != simpleArr.objectAtIndex(6)) testFail(er10);
    if (50 != simpleArr.lastObject()) testFail(er10);

    _tprintf(_T("=======================\n"));
    for (dInteger i = 0, cnt = simpleArr.count(); i < cnt; ++i)
        _tprintf(_T("item #%d = %d\n"), i, simpleArr.objectAtIndex(i));
*/
    SAFE_DELETE(alloc2);
    SAFE_DELETE(allocArr);
    SAFE_DELETE(newArr);


    // =============================================================================

    dInteger z1ms[] = { m0, m1, m2 };
    CArrayT<dInteger> z1(z1ms, 3);
    CArrayT<dInteger> z2(z1ms, 3);
    if (!z1.isEqualToArray(&z2)) testFail(er11);

//    std::auto_ptr<CArrayT<dInteger> > z3(z2.createArrayByAddingObject(m3));
//    if (z1.isEqualToArray(z3.get())) testFail(er11);

    dInteger z4ms[] = { m0, m1, m3 };
    CArrayT<dInteger> z4(z4ms, 3);
    if (z1.isEqualToArray(&z4)) testFail(er11);

    // ==============================================================================

    dInteger z5ms[] = { m4, m3, m2, m1, m0 };
    CArrayT<dInteger> z5(z5ms, 5);
    dInteger res1 = z5.firstObjectCommonWithArray(&z1);

    if (res1 != m2) testFail(er12);

    // ==============================================================================
   // z1.setArray(&z5);

     perfTest();


     // =============================================================================

     dInteger rootVal = 5, c1Val = 6, c2Val = 7, l1Val = 8, c3V = 9;

     CTreeNodeT<dInteger> root(rootVal);
     CTreeNodeT<dInteger>*   c1 = new CTreeNodeT<dInteger>(c1Val);
     CTreeNodeT<dInteger>*   c2 = new CTreeNodeT<dInteger>(c2Val);

     root.addChild(c1);
     root.addChild(c2);

     CTreeNodeT<dInteger>*  l1 = new CTreeNodeT<dInteger>(l1Val);
     c2->addChild(l1);

     CTreeNodeT<dInteger>*   c3 = new CTreeNodeT<dInteger>(c3V);
     l1->topNode()->addChild(c3);
     if (3 != root.childrenCount()) testFail(er15);
     if (2 != root.indexOfChild(c3)) testFail(er15);

     if (c3V != c3->value()) testFail(er15);
     dInteger c3Val = 10;
     c3->setValue(c3Val);
     if (c3Val != c3->value()) testFail(er15);

     CTreeNodeT<dInteger>* i1 = new CTreeNodeT<dInteger>;
     l1->addChild(i1);
     i1->setValue(c3Val);
     if (c3Val != i1->value()) testFail(er15);

     root.detachChildAtIndex(0);
     SAFE_DELETE(c1);
}

void
BaseTest::perfTest() {
    srand((unsigned int)time(nil));
/*
    CArrayT<dInteger> arr;

    dInteger n;
    for (dInteger i = 0; i < 1000; ++i) {
        dInteger num(rand() % 1000);
        if (i == 234) n = num;
        arr.addObject(num);
    }

    std::auto_ptr<CArrayT<dInteger> > arr2(new CArrayT<dInteger>(&arr));
    bool isEq = arr.isEqualToArray(arr2.get());
    if (!isEq) testFail(er11);

    TTime t1 = getTics();
    dInteger k = arr.indexOfObject(n);
    TTime t2 = getTics();
    _tprintf(_T("\nLinear search time (indexOfObject) = %d\n"), t2 - t1);

    t1 = getTics();
    arr.removeObject(n);
    t2 = getTics();
    _tprintf(_T("\nLinear search time (removeObject) = %d\n"), t2 - t1);

    isEq = arr.isEqualToArray(arr2.get());
    if (isEq) testFail(er11);

    arr2->setComparator(integerComparator, nil, true);
    arr2->sort();

    t1 = getTics();
    k = arr2->indexOfObject(n);
    t2 = getTics();
    _tprintf(_T("\nBinary search time (indexOfObject) = %d\n"), t2 - t1);

    t1 = getTics();
    arr2->removeObject(n);
    t2 = getTics();
    _tprintf(_T("\nBinary search time (removeObject) = %d\n"), t2 - t1);
*/
    //_tprintf(_T("=======================\n"));
    //for (dInteger i = 0, cnt = arr.count(); i < cnt; ++i)
    //    _tprintf(_T("item #%d = %d\n"), i, *(dInteger*)arr.objectAtIndex(i));
    templateTest();
}

void
BaseTest::templateTest() {
    dInteger cArr[] = { 89, 10, 20, 34, 50 };
    dInteger i0 = 11, i1 = 22, i2 = 33;
    CArrayT<dInteger> intArr(cArr, 5);

    intArr.addObject(i0);
    intArr.addObject(i1);

    if (20 != intArr.objectAtIndex(2)) testFail(er13);
    if (89 != intArr.objectAtIndex(0)) testFail(er13);
    if (i0 != intArr.objectAtIndex(5)) testFail(er13);
    if (i1 != intArr.lastObject()) testFail(er13);


    dInteger intArrMs[] = { i0, i1, i2 };
    CArrayT<dInteger, dInteger> intArr2(intArrMs, 3);

    if (i0 != intArr2.objectAtIndex(0)) testFail(er13);
    if (i1 != intArr2.objectAtIndex(1)) testFail(er13);
    if (i2 != intArr2.lastObject()) testFail(er13);

    // ============================================================

    TestObject* t3 = new TestObject(-3.3f, -3);
    TestObject* t4 = new TestObject(4.4f, 4);

    TestObject cTestArr[] = { TestObject(0.0f, 0), TestObject(1.1f, 1), TestObject(2.2f, 2) };

    CArrayT<TestObject> tArr(cTestArr, 3);

    // CArrayT<TestObject> tArr(*t0, *t1, *t2, *t3, *t4, nil);

    if (0.0f != *tArr.objectAtIndex(0)._fpointVal || 0 != *tArr.objectAtIndex(0)._intVal) testFail(er13);
    if (1.1f != *tArr.objectAtIndex(1)._fpointVal || 1 != *tArr.objectAtIndex(1)._intVal) testFail(er13);
    if (2.2f != *tArr.lastObject()._fpointVal || 2 != *tArr.lastObject()._intVal) testFail(er13);

    TestObject  rn[] = { TestObject(5.7f, -8), TestObject(9.9f, -9) };
    tArr.setObjectsToRange(rn, dMakeRange(2, 2));

    if (0.0f != *tArr.objectAtIndex(0)._fpointVal || 0 != *tArr.objectAtIndex(0)._intVal) testFail(er14);
    if (1.1f != *tArr.objectAtIndex(1)._fpointVal || 1 != *tArr.objectAtIndex(1)._intVal) testFail(er14);
    if (5.7f != *tArr.objectAtIndex(2)._fpointVal || -8 != *tArr.objectAtIndex(2)._intVal) testFail(er14);
    if (9.9f != *tArr.lastObject()._fpointVal || -9 != *tArr.lastObject()._intVal) testFail(er14);

    TestObject  rn2[] = { TestObject(6.6f, 6) };
    tArr.setObjectsToRange(rn2, dMakeRange(0, 1));

    if (6.6f != *tArr.objectAtIndex(0)._fpointVal || 6 != *tArr.objectAtIndex(0)._intVal) testFail(er14);
    if (1.1f != *tArr.objectAtIndex(1)._fpointVal || 1 != *tArr.objectAtIndex(1)._intVal) testFail(er14);
    if (5.7f != *tArr.objectAtIndex(2)._fpointVal || -8 != *tArr.objectAtIndex(2)._intVal) testFail(er14);
    if (9.9f != *tArr.lastObject()._fpointVal || -9 != *tArr.lastObject()._intVal) testFail(er14);

    TestObject  rn3[] = { TestObject(7.0f, 7) };
    tArr.setObjectsToRange(rn3, dMakeRange(4, 1));

    if (6.6f != *tArr.objectAtIndex(0)._fpointVal || 6 != *tArr.objectAtIndex(0)._intVal) testFail(er14);
    if (1.1f != *tArr.objectAtIndex(1)._fpointVal || 1 != *tArr.objectAtIndex(1)._intVal) testFail(er14);
    if (5.7f != *tArr.objectAtIndex(2)._fpointVal || -8 != *tArr.objectAtIndex(2)._intVal) testFail(er14);
    if (9.9f != *tArr.objectAtIndex(3)._fpointVal || -9 != *tArr.objectAtIndex(3)._intVal) testFail(er14);
    if (7.0f != *tArr.lastObject()._fpointVal || 7 != *tArr.lastObject()._intVal) testFail(er14);

    TestObject  rn4[] = { TestObject(10.0f, 0), TestObject(11.0f, 0) };
    tArr.setObjectsToRange(rn4, dMakeRange(1, 2), true);

    if (6.6f != *tArr.objectAtIndex(0)._fpointVal || 6 != *tArr.objectAtIndex(0)._intVal) testFail(er14);
    if (10.0f != *tArr.objectAtIndex(1)._fpointVal || 0 != *tArr.objectAtIndex(1)._intVal) testFail(er14);
    if (11.0f != *tArr.lastObject()._fpointVal || 0 != *tArr.lastObject()._intVal) testFail(er14);

    SAFE_DELETE(t3);
    SAFE_DELETE(t4);

}

static dComparisonResult
integerComparator(const void* obj1, const void* obj2, void* context) {
    if ((*(dInteger*)obj1) == (*(dInteger*)obj2)) return kCompareEqualTo;
    if ((*(dInteger*)obj1) > (*(dInteger*)obj2)) return kCompareGreaterThan;
    if ((*(dInteger*)obj1) < (*(dInteger*)obj2)) return kCompareLessThan;

    return !kCompareEqualTo;
}