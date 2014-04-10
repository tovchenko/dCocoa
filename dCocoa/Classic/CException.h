/*
** $Id: 2009/02/07, Taras Tovchenko $
** Basic exception class and some generic exceptions
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_EXCEPTION_H
#define _COCOA_EXCEPTION_H


#if defined(DEBUG)
	#define CL_THROW_IF_NIL(condition)	{ if (!(condition)) throw new CL::ExAssert; }
	#define CL_THROW_IF_TRUE(condition)	{ if ((condition)) throw new CL::ExAssert; }
	#define CL_THROW_NOT_IMP()			{ throw new CL::ExNotImplemented; }
#else
	#define CL_THROW_IF_NIL(condition)
	#define CL_THROW_IF_TRUE(condition)
	#define CL_THROW_NOT_IMP()
#endif


BEGIN_NAMESPACE_CL

class DLL_PUBLIC CException {
public:
    CException();
    // all exception objects must be created on heap, and deleted
protected:
    virtual ~CException();
};

class DLL_PUBLIC ExAssert : public CException { public: ExAssert(); protected: virtual ~ExAssert(); };
class DLL_PUBLIC ExNotImplemented : public CException { public: ExNotImplemented(); protected: virtual ~ExNotImplemented(); };

END_NAMESPACE_CL

#endif // _COCOA_EXCEPTION_H