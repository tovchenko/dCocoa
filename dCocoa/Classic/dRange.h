/*
** $Id: 2009/02/07, Taras Tovchenko $
** Range struct type which using in all collection and algorithms
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DRANGE_H
#define _COCOA_DRANGE_H

#define dRangeCheck(_range_, _count_)     \
    if ((_range_).location >= (_count_))    \
        throw new ExRange((_range_).location);    \
    if ((_range_).length + (_range_).location > (_count_))    \
        throw new ExRange((_range_).length + (_range_).location);


BEGIN_NAMESPACE_CL

typedef struct _dRange {
    dInteger location;
    dInteger length;
} dRange;

extern DLL_PUBLIC const dRange dZeroRange;

inline dRange
dMakeRange(dInteger loc, dInteger len) {
    dRange range = { loc, len };
    return range;
}

class DLL_PUBLIC ExRange : public CException {
public:
    ExRange(dInteger wrongIndex);

    dInteger        wrongIndex() const;

protected:
    dInteger        _wrongIndex;
};



END_NAMESPACE_CL

#endif // _COCOA_DRANGE_H
