/*
** $Id: 2009/02/07, Taras Tovchenko $
** Generic algorithms witch can be used in different targets
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_ALGORITHMS_H
#define _COCOA_ALGORITHMS_H


BEGIN_NAMESPACE_CL

namespace uALGO {

void DLL_PUBLIC
    qSort(
        void* base,
        dInteger itemsCount,
        dUInteger itemSize,
        dComparatorFunction comparatorCallback,
        void* comparatorContext);
 // if 'direction' == 'true' than forward, owerwise backward
dInteger DLL_PUBLIC
    linearSearch(
        const void* base,
        const void* object,
        dInteger startPosition,
        bool direction,
        dInteger itemsCount,
        dUInteger itemSize,
        dComparatorFunction comparatorCallback,
        void* comparatorContext);

dInteger DLL_PUBLIC
    binarySearch(
        const void* base,
        const void* object,
        dInteger startPosition,
        bool direction,
        dInteger itemsCount,
        dUInteger itemSize,
        dComparatorFunction comparatorCallback,
        void* comparatorContext);

dComparisonResult DLL_PUBLIC
    defaultComparator(
        dUInteger itemSize,
        const void* obj1,
        const void* obj2);

dUInteger DLL_PUBLIC
    hashFromString(const char* str);

dUInteger DLL_PUBLIC
    hashFromByteSequence(const dByte* buffer, dUInteger size);

}



class DLL_PUBLIC ExComparator : public CException {
};

END_NAMESPACE_CL

#endif // _COCOA_ALGORITHMS_H