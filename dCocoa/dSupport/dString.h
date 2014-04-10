/*
** $Id: 2009/07/10, Taras Tovchenko $
** String class, converters
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DSTRING_H
#define _COCOA_DSTRING_H


#define _ST(chars)			CL::dString::stringWithCharacters(_T(chars))


BEGIN_NAMESPACE_CL

class dData;

enum {
	kECaseInsensitiveSearch = 1 << 0,
	kENumericSearch = 1 << 1,
	kELiteralSearch = 1 << 2
};
typedef dInteger	dEStringCompareOptions;

class DLL_PUBLIC dString : public dObject {
CL_CLASS_DECL(dString, dObject)
public:
	// exceptions
	class DLL_PUBLIC ExEmptyString : public CException { };
	// TCHAR*/dString to char*
	class DLL_PUBLIC MultiByte {
	public:
        explicit MultiByte(const TCHAR* wstr) { _init(wstr); }
        explicit MultiByte(const dString* dstr) { _init(dstr ? dstr->chars() : nil); }
		~MultiByte() { SAFE_DELETE_ARRAY(_mb); }
		operator const char*() const { return _mb; }
		
	private:
        void        _init(const TCHAR* wstr);
		char*		_mb;
	};
    // char* to TCHAR*/dString
    class DLL_PUBLIC SingleByte {
	public:
        explicit SingleByte(const char* str) { _init(str); }
		~SingleByte() { SAFE_DELETE_ARRAY(_mb); }
		operator const TCHAR*() const { return _mb; }
        operator dString*() const { return dString::stringWithCharacters(_mb); }
		
	private:
        void        _init(const char* str);
		TCHAR*		_mb;
	};
	// creating
	static	dString*			string();
	static	dString*			stringWithCharacters(const TCHAR* chars, dUInteger lngth = kNotDefined);
    static  dString*            stringWithString(const dString* str);
	static	dString*			stringWithData(dData* data);
	
    virtual dString*            initWithCharacters(const TCHAR* chars, dUInteger lngth = kNotDefined);
    virtual dString*            initWithString(const dString* str);
	virtual dString*			initWithData(dData* data);

    virtual dString*            copy() const;

    const TCHAR*                chars() const;
    dUInteger                   length() const;
	// getting characters
	TCHAR						characterAtIndex(dUInteger index) const;
	void						getCharacters(TCHAR* outChars, dRange range = dZeroRange) const;
	// identifying and comparing strings
	dComparisonResult			caseInsensitiveCompare(const dString* inStr) const;
	dComparisonResult			compare(const dString* inStr, dEStringCompareOptions options = kELiteralSearch, dRange range = dZeroRange) const;
	bool						hasPrefix(const dString* prefix) const;
	bool						hasSuffix(const dString* suffix) const;
    // hash & equal
    virtual dInteger            hash() const;
    virtual bool                isEqual(const dObject* otherObject) const;
	// changing case
	dString*					capitalizedString() const;
	dString*					lowercaseString() const;
	dString*					uppercaseString() const;
	// working with paths
	static dString*				pathWithComponents(dArrayT<dString*>* components);
	dArrayT<dString*>*			pathComponents() const;
	bool						isAbsolutePath() const;
	dString*					lastPathComponent() const;
	dString*					pathExtension() const;
	dString*					stringByAppendingPathComponent(const dString* component) const;
	dString*					stringByAppendingPathExtension(const dString* extension) const;
	dString*					stringByDeletingLastPathComponent() const;
	dString*					stringByDeletingPathExtension() const;
	dArrayT<dString*>*			stringsByAppendingPaths(dArrayT<dString*>* paths) const;

protected:
    dString();
    virtual void                dealloc();
	
	void                        _setValueToRange(const TCHAR* chars, dRange rng, bool needsDeleteTail = false);
    
    CArrayT<TCHAR>*             _chars;
};


class DLL_PUBLIC dStaticString : public dString {
public:
    dStaticString(const TCHAR* chars) { initWithCharacters(chars); }
    ~dStaticString() { dString::dealloc(); }
};

END_NAMESPACE_CL

#endif // _COCOA_DSTRING_H