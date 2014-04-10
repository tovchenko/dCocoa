/*
** $Id: 2009/03/30, Taras Tovchenko $
** Basic struct types for describe point, rect, etc.
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DGRAPHICS2D_H
#define _COCOA_DGRAPHICS2D_H



BEGIN_NAMESPACE_CL

typedef struct _dPoint {
    dFloat   x;
    dFloat   y;
} dPoint;

extern DLL_PUBLIC const dPoint dZeroPoint;

inline dPoint
dMakePoint(dFloat x, dFloat y) {
    dPoint point = { x, y };
    return point;
}


typedef struct _dSize {
    dFloat   width;
    dFloat   height;
} dSize;

extern DLL_PUBLIC const dSize dZeroSize;

inline dSize
dMakeSize(dFloat width, dFloat height) {
    dSize size = { width, height };
    return size;
}


typedef struct _dRect {
    dPoint  origin;
    dSize   size;
} dRect;

extern DLL_PUBLIC const dRect dZeroRect;

inline dRect
dMakeRect(dFloat x, dFloat y, dFloat width, dFloat height) {
    dRect rect = { { x, y }, { width, height } };
    return rect;
}

END_NAMESPACE_CL

#endif // _COCOA_DGRAPHICS2D_H
