/*
 ** $Id: 2009/02/07, Taras Tovchenko $
 ** Unit Tests
 ** See Copyright Notice in dCocoa.h
 */

#ifdef IS_MAC
#import <Foundation/Foundation.h>
#endif

#include "stdafx.h"
#include "TestClasses.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#include <conio.h>

#pragma data_seg(".CRT$XIU")
static int __memBlockBreak();
static int (*__memBlockBreakPtr)() = __memBlockBreak;
#pragma data_seg()
int
__memBlockBreak() {
    // TODO: uncomment this and past number of memory block
    // when you detect memory leak
    // _CrtSetBreakAlloc(1196);
    return 0;
}
#endif

int
main(int argc, char* argv[]) {
#ifdef _MSC_VER
    int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flags |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flags);
#else
	NSAutoreleasePool* pool = [NSAutoreleasePool new];
#endif
	
    _LocalPool
	
    BaseTest::run();
    DictionaryTest::run();
    SetTest::run();
    StringTest::run();
    DObjectTest::run();
    NotifTest::run();
	
#ifdef _MSC_VER
    _getch();
#else
	[pool drain];
#endif
	
	return 0;
}