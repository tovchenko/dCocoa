/*
** $Id: 2009/02/07, Taras Tovchenko $
** Unit Tests
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "TestClasses.h"


TEST_MSG_INFO(er0, "Don\'t work dString!\n");
TEST_MSG_INFO(er1, "Don\'t work dString::hasPrefix!\n");
TEST_MSG_INFO(er2, "Don\'t work dString::compare!\n");
TEST_MSG_INFO(er3, "Don\'t work dString:: case changing!\n");
TEST_MSG_INFO(er4, "Don\'t work dString:: path!\n");



void
StringTest::run() {
    dString* str = _ST("Test-String!a");
    if (13 != str->length()) testFail(er0);
//    dString::MultiByte str8 = dString::MultiByte(str);
//    const char* utf8Str = str8;

	bool res = str->hasPrefix(_ST("Test"));
	if (!res) testFail(er1);
	res = str->hasPrefix(_ST("Testi"));
	if (res) testFail(er1);
	res = str->hasPrefix(_ST("Test-String!a"));
	if (!res) testFail(er1);
	
	dString* str2 = _ST("test-string!a");
	dComparisonResult cres  = str->caseInsensitiveCompare(str2);
	if (kCompareEqualTo != cres) testFail(er2);
	
	dString* str3 = _ST("test-string!b");
	cres  = str->caseInsensitiveCompare(str3);
	if (kCompareGreaterThan != cres) testFail(er2);
	
	dString* str4 = _ST("128");
	dString* str5 = _ST("95");
	cres = str5->compare(str4, kENumericSearch);
	if (kCompareLessThan == cres) testFail(er2);
	
	cres = str4->compare(str5, kENumericSearch, dMakeRange(0, 2));
	if (kCompareLessThan == cres) testFail(er2);
	
	dString* lowS = _ST("hello");
	res = lowS->uppercaseString()->isEqual(_ST("HELLO"));
	if (!res) testFail(er3);
	
	
	dArrayT<dString*>* components = CL_NEW(dArrayT<dString*>)->autorelease();
	components->addObject(_ST("Documents/"));
	components->addObject(_ST("/Secondary/"));
	components->addObject(_ST("/"));
	components->addObject(_ST("third/"));
	components->addObject(_ST("/"));
	
	dString* path = dString::pathWithComponents(components);
	if (!path->isEqual(_ST("Documents/Secondary/third"))) testFail(er4);
	
	components = path->pathComponents();
	if (!components->objectAtIndex(0)->isEqual(_ST("Documents"))) testFail(er4);
	if (!components->objectAtIndex(1)->isEqual(_ST("Secondary"))) testFail(er4);
	if (!components->objectAtIndex(2)->isEqual(_ST("third"))) testFail(er4);
	
	if (path->isAbsolutePath()) testFail(er4);
	if (!_ST("/path/blablabla")->isAbsolutePath()) testFail(er4);
	
	if (!path->lastPathComponent()->isEqual(_ST("third"))) testFail(er4);
	
	dString* fileName = path->stringByAppendingPathExtension(_ST("jpg"));
	if (!fileName->pathExtension()->isEqual(_ST("jpg"))) testFail(er4);
	if (!fileName->stringByDeletingPathExtension()->isEqual(path)) testFail(er4);
	
	dString* apPath = path->stringByAppendingPathComponent(_ST("HoHo"));
	if (!apPath->isEqual(_ST("Documents/Secondary/third/HoHo"))) testFail(er4);
	if (!apPath->stringByDeletingLastPathComponent()->isEqual(path)) testFail(er4);
	
	dArrayT<dString*>* aps = CL_NEW(dArrayT<dString*>)->autorelease();
	aps->addObject(_ST("/Level1/Level2"));
	aps->addObject(_ST("HoHoHo"));
	dArrayT<dString*>* apPaths = path->stringsByAppendingPaths(aps);
	
	if (!apPaths->objectAtIndex(0)->isEqual(_ST("Documents/Secondary/third/Level1/Level2"))) testFail(er4);
	if (!apPaths->objectAtIndex(1)->isEqual(_ST("Documents/Secondary/third/HoHoHo"))) testFail(er4);
	
}