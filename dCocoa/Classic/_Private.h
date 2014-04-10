/*
** $Id: 2009/02/07, Taras Tovchenko $
** Main header, implement basic types, macroses, namespaces, definitions
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_PRIVATE_H
#define _COCOA_PRIVATE_H



#if defined(__APPLE_CC__)
    #define IS_MAC
#elif defined(_MSC_VER) || defined(__CYGWIN__)
    #define IS_WIN
#endif


#if defined(IS_MAC)
    #define _UNICODE
    #define _WCTYPE_T_DEFINED
    #import "tchar.h"
    #ifdef __OBJC__
        #import <Foundation/Foundation.h>
    #endif
#endif


#if defined(IS_WIN)
	#ifndef BUILDING_DLL
		#define BUILDING_DLL 1
	#endif

	#if BUILDING_DLL
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__((dllexport))
		#else
			#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
		#endif
	#else
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__((dllimport))
		#else
			#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
		#endif
	#endif
	#define DLL_LOCAL
#else
	#if __GNUC__ >= 4
		#define DLL_PUBLIC __attribute__ ((visibility("default")))
		#define DLL_LOCAL  __attribute__ ((visibility("hidden")))
	#else
		#define DLL_PUBLIC
		#define DLL_LOCAL
	#endif
#endif



#ifndef USE_CL_NAMES
    #define USE_CL_NAMES 1
#endif
#if USE_CL_NAMES
    #define BEGIN_NAMESPACE_CL       namespace CL {
    #define END_NAMESPACE_CL         }
    #define USING_NAMESPACE_CL       using namespace CL;
    #define CL                       dCocoa
#else
    #define BEGIN_NAMESPACE_CL
    #define END_NAMESPACE_CL
    #define USING_NAMESPACE_CL
    #define CL
#endif


#ifndef COCOA_TYPES
    #define COCOA_TYPES 1
#endif
#if COCOA_TYPES
    #if defined(IS_WIN)
        #include <BaseTsd.h>
        #include <tchar.h>

        typedef INT_PTR         dInteger;
        typedef UINT_PTR        dUInteger;
        typedef float           dFloat;
        typedef signed char     dByte;
    #elif defined(IS_MAC)
    #import <QuartzCore/CABase.h>

		#ifdef __OBJC__
			typedef NSInteger       dInteger;
			typedef NSUInteger      dUInteger;
		#else
			typedef int				dInteger;
			typedef unsigned int    dUInteger;
		#endif
		typedef CGFloat         dFloat;
		typedef signed char     dByte;
	#endif
#else
    typedef int                 dInteger;
    typedef unsigned int        dUInteger;
    typedef float               dFloat;
    typedef signed char         dByte;
#endif

// comparison interface
enum {
    kCompareLessThan = -1,
    kCompareEqualTo = 0,
    kCompareGreaterThan = 1
};
typedef dInteger            dComparisonResult;
typedef dComparisonResult   (*dComparatorFunction)(const void* obj1, const void* obj2, void* context);

enum {
    kNotFound = -1,
    kNotDefined = -1
};

enum {
    kFixLen = 255
};

extern DLL_PUBLIC const dByte __kNull;

#define NIL_OBJ			((void*)&__kNull)

#ifndef nil
    #define nil    0
#endif

#define SAFE_DELETE(p)                  { if (p) { delete (p); (p) = nil; } }
#define SAFE_DELETE_ARRAY(p)            { if (p) { delete[] (p); (p) = nil; } }

#if defined(IS_WIN)
    #define SAFE_RELEASE(p)             { if (p) { (p)->Release(); (p) = nil; } }
#endif

#define ZERO_MEMORY(ptr, size)          memset(ptr, 0, size)
#define ZERO_MEMORY_STRUCT(st)          ZERO_MEMORY(&st, sizeof(st))
#define ZERO_MEMORY_ARRAY(arr)          ZERO_MEMORY(arr, sizeof(arr))

#define ITEMS_COUNT_FROM_ARRAY(arr)     (sizeof(arr) / sizeof(arr[0]))

#if defined(IS_WIN)
    #define PASCAL      __stdcall
#elif defined(IS_MAC)
    #define PASCAL
#endif

#if !defined(__max) && !defined(__min)
    #define __max(a,b)  (((a) > (b)) ? (a) : (b))
    #define __min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#define _TO_STR(lexem)          #lexem


#endif // _COCOA_PRIVATE_H