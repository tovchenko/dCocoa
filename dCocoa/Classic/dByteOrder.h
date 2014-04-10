/*
 ** $Id: 2010/12/15, Taras Tovchenko $
 ** Byte ordering
 ** See Copyright Notice in dCocoa.h
 */

#ifndef _COCOA_BYTEORDER_H
#define _COCOA_BYTEORDER_H


//---------------------------- Intel X86
#if defined(__i386__)  && defined(__GNUC__)//GNU GCC

#ifdef __LITTLE_ENDIAN__
#undef		__LITTLE_ENDIAN__
#define		__LITTLE_ENDIAN__ 1
#endif//__LITTLE_ENDIAN__

#ifndef __LITTLE_ENDIAN__
#define		__LITTLE_ENDIAN__ 1
#endif//__LITTLE_ENDIAN__

#ifdef __BIG_ENDIAN__
#undef __BIG_ENDIAN__
#endif	//__BIG_ENDIAN__

#endif //__i386__

#if     defined(_M_IX86) && defined(_MSC_VER)
/*
_M_IA64		- Intel IA64	-	LE
_M_X64		- AMD 64		-	LE

_M_MRX000	- SGI MIPS		-	BE
_M_ALPHA	- DEC Alpha		-	LE
_M_PPC		- IBM PPC		-	BE

*/
#ifndef __LITTLE_ENDIAN__
#define		__LITTLE_ENDIAN__ 1
#endif//__LITTLE_ENDIAN__
#endif	//_M_IX86


//--------------------------- IBM PPC
#if defined(__ppc__) && defined(__GNUC__)
#ifndef __BIG_ENDIAN__
#define		__BIG_ENDIAN__ 1
#endif//__BIG_ENDIAN__

#ifdef __LITTLE_ENDIAN__
#undef __LITTLE_ENDIAN__
#endif	//__LITTLE_ENDIAN__

#endif //__ppc__


#if defined(__cplusplus)
extern "C" {
#endif

enum _dByteOrder {
	kUnknownByteOrder,
	kLittleEndian,
	kBigEndian
};

typedef struct {
	unsigned long long v;
} dSwappedDouble;

typedef struct {
	unsigned long v;
} dSwappedFloat;


union dSwapFloatUn {
	float v;
	dSwappedFloat sv;
};

union dSwapDoubleUn {
	double v;
	dSwappedDouble sv;
};

inline _dByteOrder dByteOrderGetCurrent() {
	unsigned int x = (kBigEndian << 24) | kLittleEndian;
	return (_dByteOrder)*((char *)&x);
}

inline unsigned short _dSwapInt16(unsigned short x) {
#if defined(__i386__) && defined(__GNUC__)
	__asm__("xchgb %b0, %h0" : "+q" (x));
	return x;
#elif defined(__ppc__) && defined(__GNUC__)
	uint16_t result;
	__asm__("lhbrx %0,0,%1" : "=r" (result) : "r" (&x), "m" (x));
	return result;
#else
	unsigned short result;
	result = ((x << 8) & 0xFF00) | ((x >> 8) & 0xFF);
	return result;
#endif
}

inline unsigned int _dSwapInt32(unsigned int  x) {
#if defined(__i386__) && defined(__GNUC__)
	__asm__("bswap %0" : "+r" (x));
	return x;
#elif defined(__ppc__) && defined(__GNUC__)
	uint32_t result;
	__asm__("lwbrx %0,0,%1" : "=r" (result) : "r" (&x), "m" (x));
	return result;
#else
	unsigned int result;
	result = ((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | ((x >> 24) & 0xFF);
	return result;
#endif
}

inline unsigned long long _dSwapInt64(unsigned long long  x) {	//VS6.0?
	union CFSwap {
		unsigned long long sv;
		unsigned int ul[2];
	} tmp, result;
	tmp.sv = x;
	result.ul[0] = _dSwapInt32(tmp.ul[1]); 
	result.ul[1] = _dSwapInt32(tmp.ul[0]);
	return result.sv;
}

//----- current platform ----
inline unsigned int		dHostByteOrder() {
	return dByteOrderGetCurrent();
}

//-----------------------------------  swap big-endian to host ---------------

inline unsigned long long	dSwapBigLongLongToHost(unsigned long long x)
{
#if defined(__BIG_ENDIAN__)
	return x;
#else
	return _dSwapInt64(x);
#endif
}

inline unsigned long		dSwapBigLongToHost(unsigned long x)		//! \todo 64bit Win/Linux portability
{
#if defined(__BIG_ENDIAN__)
	return x;
#elif defined(_64_BIT)
	return _dSwapInt64(x);
#else
	return _dSwapInt32(x);
#endif
}

inline unsigned int		dSwapBigIntToHost(unsigned int x)
{
#if defined(__BIG_ENDIAN__)
	return x;
#else
	return _dSwapInt32(x);
#endif
}

inline unsigned short		dSwapBigShortToHost(unsigned short x)
{
#if defined(__BIG_ENDIAN__)
	return x;
#else
	return _dSwapInt16(x);
#endif
}
/*
dSwapDoubleUn  result;
result.sv = x;
#if defined(__LITTLE_ENDIAN__)
result.sv.v = _dSwapInt64(result.sv.v);
#endif
return result.v;
*/
inline double				dSwapBigDoubleToHost(dSwappedDouble x)
{
	dSwapDoubleUn  result;
	result.sv		= x;
#if defined(__BIG_ENDIAN__)
	//do nothing
#else
	result.sv.v = _dSwapInt64(result.sv.v);
#endif
	return result.v;
}
inline float				dSwapBigFloatToHost(dSwappedFloat x)
{
	dSwapFloatUn  result;
	result.sv		= x;
#if defined(__BIG_ENDIAN__)
	//do nothing
#else
	result.sv.v = _dSwapInt32(result.sv.v);
#endif
	return result.v;
}


//--------------------------- swap host to bigEndian -------------------------------

inline unsigned long long	dSwapHostLongLongToBig(unsigned long long x)
{
#if defined(__BIG_ENDIAN__)
	return x;
#else
	return _dSwapInt64(x);
#endif
}

inline unsigned int		dSwapHostIntToBig(unsigned int x)
{
#if defined(__BIG_ENDIAN__)
	return x;
#else
	return _dSwapInt32(x);
#endif
}
inline unsigned long		dSwapHostLongToBig(unsigned long x)
{
#if defined(__BIG_ENDIAN__)
	return x;
#elif defined(_64_BIT)
	return _dSwapInt64(x);
#else
	return _dSwapInt32(x);
#endif
}

inline unsigned short		dSwapHostShortToBig(unsigned short x)
{
#if defined(__BIG_ENDIAN__)
	return x;
#else
	return _dSwapInt16(x);
#endif
}

inline dSwappedDouble		dSwapHostDoubleToBig(double x)
{
	dSwapDoubleUn  result;
	result.v		= x;
#if defined(__BIG_ENDIAN__)
	//do nothing
#else
	result.sv.v = _dSwapInt64(result.sv.v);
#endif
	return result.sv;
}
inline dSwappedFloat		dSwapHostFloatToBig(float x)
{
	dSwapFloatUn  result;
	result.v		= x;
#if defined(__BIG_ENDIAN__)
	//do nothing
#else
	result.sv.v = _dSwapInt32(result.sv.v);
#endif
	return result.sv;
}

//---------------------------- swap little to host -------------------------------
inline unsigned long long	dSwapLittleLongLongToHost(unsigned long long x)
{
#if defined(__LITTLE_ENDIAN__)
	return x;
#else
	return _dSwapInt64(x);
#endif
}
inline unsigned int		dSwapLittleIntToHost(unsigned int x)
{
#if defined(__LITTLE_ENDIAN__)
	return x;
#else
	return _dSwapInt32(x);
#endif
}

inline unsigned long		dSwapLittleLongToHost(unsigned long x)
{
#if defined(__LITTLE_ENDIAN__)
	return x;
#elif defined(_64_BIT)
	return _dSwapInt64(x);
#else
	return _dSwapInt32(x);
#endif
}

inline unsigned short		dSwapLittleShortToHost(unsigned short x)
{
#if defined(__LITTLE_ENDIAN__)
	return x;
#else
	return _dSwapInt16(x);
#endif
}

inline float				dSwapLittleFloatToHost(dSwappedFloat x)
{
	dSwapFloatUn  result;
	result.sv		= x;
#if defined(__LITTLE_ENDIAN__)
	//do nothing
#else
	result.sv.v = _dSwapInt32(result.sv.v);
#endif
	return result.v;
}
inline double				dSwapLittleDoubleToHost(dSwappedDouble x)
{
	dSwapDoubleUn  result;
	result.sv		= x;
#if defined(__LITTLE_ENDIAN__)
	//do nothing
#else
	result.sv.v = _dSwapInt64(result.sv.v);
#endif
	return result.v;
}

//---------------------------- swap host to little -------------------------------
inline unsigned long long	dSwapHostLongLongToLittle(unsigned long long x)
{
#if defined(__LITTLE_ENDIAN__)
	return x;
#else
	return _dSwapInt64(x);
#endif
}
inline unsigned int		dSwapHostIntToLittle(unsigned int x)
{
#if defined(__LITTLE_ENDIAN__)
	return x;
#else
	return _dSwapInt32(x);
#endif
}
inline unsigned long		dSwapHostLongToLittle(unsigned long x)
{
#if defined(__LITTLE_ENDIAN__)
	return x;
#elif defined(_64_BIT)
	return _dSwapInt64(x);
#else
	return _dSwapInt32(x);
#endif
}
inline unsigned short		dSwapHostShortToLittle(unsigned short x)
{
#if defined(__LITTLE_ENDIAN__)
	return x;
#else
	return _dSwapInt16(x);
#endif
}

inline dSwappedDouble		dSwapHostDoubleToLittle(double x)
{
	dSwapDoubleUn  result;
	result.v		= x;
#if defined(__LITTLE_ENDIAN__)
	//do nothing
#else
	result.sv.v = _dSwapInt64(result.sv.v);
#endif
	return result.sv;
}
inline dSwappedFloat		dSwapHostFloatToLittle(float x)
{
	dSwapFloatUn  result;
	result.v		= x;
#if defined(__LITTLE_ENDIAN__)
	//do nothing
#else
	result.sv.v = _dSwapInt32(result.sv.v);
#endif
	return result.sv;
}

//-------------------------------- fp ------------
inline dSwappedFloat		dConvertHostFloatToSwapped(float x)
{
	dSwapFloatUn result;
	result.v = x;
#if defined(__LITTLE_ENDIAN__)
	result.sv.v = _dSwapInt32(result.sv.v);
#endif
	return result.sv;
}

inline float				dConvertSwappedFloatToHost(dSwappedFloat x)
{
	dSwapFloatUn result;
	result.sv = x;
#if defined(__LITTLE_ENDIAN__)
	result.sv.v = _dSwapInt32(result.sv.v);
#endif
	return result.v;
}



inline dSwappedDouble		dConvertHostDoubleToSwapped(double x)
{
	dSwapDoubleUn result;
	result.v = x;
#if defined(__LITTLE_ENDIAN__)
	result.sv.v = _dSwapInt64(result.sv.v);
#endif
	return result.sv;
}

inline double				dConvertSwappedDoubleToHost(dSwappedDouble x)
{
	dSwapDoubleUn  result;
	result.sv = x;
#if defined(__LITTLE_ENDIAN__)
	result.sv.v = _dSwapInt64(result.sv.v);
#endif
	return result.v;
}


//--------------------- Simple Swap ---------------------------------------
inline unsigned short		dSwapShort (unsigned short inv)
{
	return _dSwapInt16(inv);
}

inline unsigned int		dSwapInt (unsigned int inv)
{
	return _dSwapInt32(inv);
}

inline unsigned long		dSwapLong(unsigned long inv) 
{
#if defined(_64_BIT)
	return _dSwapInt64(inv);
#else
	return _dSwapInt32(inv);
#endif
}

inline unsigned long long	dSwapLongLong(unsigned long long inv)
{
	return _dSwapInt64(inv);
}

inline dSwappedDouble		dSwapDouble(dSwappedDouble x)
{
	dSwapDoubleUn result;
	result.sv = x;
	result.sv.v = _dSwapInt64(result.sv.v);
	return result.sv;
}
inline dSwappedFloat		dSwapFloat(dSwappedFloat x)
{
	dSwapFloatUn result;
	result.sv = x;
	result.sv.v = _dSwapInt32(result.sv.v);
	return result.sv;
}


#if defined(__cplusplus)
}
#endif

#endif //_COCOA_BYTEORDER_H

