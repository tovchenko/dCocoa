/*
** $Id: 2009/07/10, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dString.h"
#include "dArray.h"
#include "dData.h"

#include <locale.h>
#include <stdlib.h>

#define IS_SLASH(X)  ((X) == _T('/') || (X) == _T('\\'))

#if defined(IS_WIN)
	#define SLASH _T('\\')
	#define	SLASH_STR _T("\\")
#else
	#define SLASH _T('/')
	#define	SLASH_STR _T("/")
#endif


typedef unsigned short	ucs2char;
typedef unsigned long	utf32char;


static dStaticString	_kSlashString(SLASH_STR);

static char*		_wcharToLocale(const wchar_t* str);
static wchar_t*		_localeToWchar(const char* str);
static TCHAR		_toUpperCase(TCHAR character);
static TCHAR		_toLowerCase(TCHAR character);
static bool			_isNumChar(TCHAR character);
static void			_upperCaseString(TCHAR* characters);
static void			_lowerCaseString(TCHAR* characters);
static void			_capitalizedString(TCHAR* characters);
static dInteger		_ctoi(TCHAR* c, dUInteger* len);
static void			_convertUCS2ToUTF32(ucs2char const* src, utf32char* dest, dUInteger chCount);

CL_CLASS_DEF(dString, dCocoa)


void
dString::MultiByte::_init(const TCHAR* wstr) {
    if (!wstr) return;
	
	try {
#if defined(_UNICODE)
		_mb = _wcharToLocale(wstr);
#else
		_mb = new char[_tcslen(wstr) + 1];
		strcpy(_mb, wstr);
#endif
	} catch (...) {
		SAFE_DELETE_ARRAY(_mb);
		throw;
	}
}

void
dString::SingleByte::_init(const char* str) {
    if (!str) return;
	
	try {
#if defined(_UNICODE)
		_mb = _localeToWchar(str);
#else
		_mb = new char[_tcslen(str) + 1];
		strcpy(_mb, str);
#endif
	} catch (...) {
		SAFE_DELETE_ARRAY(_mb);
		throw;
	}
}


dString::dString()
: _chars(nil) {
}

dString*
dString::string() {
	return _ST("");
}

dString*
dString::stringWithCharacters(const TCHAR* chars, dUInteger lngth) {
	_self* s = alloc()->initWithCharacters(chars, lngth);
	return s ? s->autorelease() : nil;
}

dString*
dString::stringWithString(const dString* str) {
	_self* s = alloc()->initWithString(str); return s ? s->autorelease() : nil;
}

dString*
dString::stringWithData(dData* data) {
	_self* s = alloc()->initWithData(data); return s ? s->autorelease() : nil;
}

dString*
dString::initWithCharacters(const TCHAR* chars, dUInteger lngth) { 
    if (!_super::init()) return nil;
    _setValueToRange(chars, dMakeRange(0, lngth), true);
    return this;
}

dString*
dString::initWithString(const dString* str) {
    if (!_super::init()) return nil;
    _setValueToRange(str->chars(), dMakeRange(0, str->length()), true);
    return this;
}

dString*
dString::initWithData(dData* data) {
#if defined(IS_WIN)
	return initWithCharacters((const TCHAR*)data->bytes(), data->length() / sizeof(TCHAR));
#else
	const dUInteger len = 2 * data->length();
	const dUInteger charCount = len / sizeof(TCHAR);
	dByte utf32Str[len];
	
	_convertUCS2ToUTF32((const ucs2char*)data->bytes(), (utf32char*)utf32Str, charCount);
	return initWithCharacters((const TCHAR*)utf32Str, charCount);
#endif
}

dString*
dString::copy() const {
	return dString::alloc()->initWithString(this);
}

void
dString::dealloc() {
    SAFE_DELETE(_chars);
    _super::dealloc();
}

const TCHAR*
dString::chars() const {
    return _chars ? &_chars->objectAtIndex(0) : nil;
}

dUInteger
dString::length() const {
    return _chars ? _chars->count() - 1 : 0;
}

TCHAR
dString::characterAtIndex(dUInteger index) const {
	if (!_chars) throw new ExEmptyString;
	return _chars->objectAtIndex(index);
}

void
dString::getCharacters(TCHAR* outChars, dRange range) const {
	if (!_chars) throw new ExEmptyString;
	if (dZeroRange.location == range.location && dZeroRange.length == range.length)
		range = dMakeRange(0, length());
	_chars->getObjectsInRange(outChars, range);
	outChars[range.length] = _T('\0');
}

dComparisonResult
dString::caseInsensitiveCompare(const dString* inStr) const {
	return compare(inStr, kECaseInsensitiveSearch);
}

dComparisonResult
dString::compare(const dString* inStr, dEStringCompareOptions options, dRange range) const {
	if (!inStr) return kCompareLessThan;
	if (dZeroRange.location == range.location && dZeroRange.length == range.length)
		range = dMakeRange(0, length());
	
	dUInteger inStrLen = inStr->length();
	TCHAR selfBuff[range.length + 1];
	TCHAR inStrBuff[inStrLen + 1];
	
	getCharacters(selfBuff, range);
	inStr->getCharacters(inStrBuff);
	
	if (kECaseInsensitiveSearch & options) {
		_upperCaseString(selfBuff);
		_upperCaseString(inStrBuff);
	}
	
	dInteger numResult;
	dUInteger i, j, il, jl;
	if (kENumericSearch & options) {
		for (i = 0, j = 0; i < range.length && j < inStrLen; ++i, ++j) {
			if (_isNumChar(selfBuff[i]) && _isNumChar(inStrBuff[j])) {
				il = range.length;
				jl = inStrLen; 
				numResult = _ctoi(&selfBuff[i], &il) - _ctoi(&inStrBuff[j], &jl);
				if (numResult < 0 ) return kCompareGreaterThan;
				else if (numResult > 0) return kCompareLessThan;
				i += il;
				j += jl;
			}
			
			if (selfBuff[i] < inStrBuff[j]) return kCompareGreaterThan;
			else if (selfBuff[i] > inStrBuff[j]) return kCompareLessThan;
		}
	} else {
		for (i = 0; i < range.length && i < inStrLen; ++i) {
			if (selfBuff[i] < inStrBuff[i]) return kCompareGreaterThan;
			else if (selfBuff[i] > inStrBuff[i]) return kCompareLessThan;
		}
	}
	
	if (range.length == inStrLen)
		return kCompareEqualTo;
	
	return i < inStrLen ? kCompareGreaterThan : kCompareLessThan;
}

bool
dString::hasPrefix(const dString* prefix) const {
	CL_THROW_IF_NIL(prefix);
	dUInteger selfLen = length();
	dUInteger prefixLen = prefix->length();
	if (prefixLen > selfLen || !_chars)
		return false;
	
	return !memcmp(chars(), prefix->chars(), prefixLen * sizeof(TCHAR));
}

bool
dString::hasSuffix(const dString* suffix) const {
	CL_THROW_IF_NIL(suffix);
	dUInteger selfLen = length();
	dUInteger suffixLen = suffix->length();
	if (suffixLen > selfLen || !_chars)
		return false;
	
	return !_tcscmp(&_chars->objectAtIndex(selfLen - suffixLen), suffix->chars());
}

dInteger
dString::hash() const {
    return uALGO::hashFromByteSequence((dByte*)chars(), length() * sizeof(TCHAR));
}

bool
dString::isEqual(const dObject* otherObject) const {
	if (_super::isEqual(otherObject))
		return true;
	
    return !_tcscmp(chars(), ((dString*)otherObject)->chars());
}

dString*
dString::capitalizedString() const {
	if (!_chars) throw new ExEmptyString;
	
	dUInteger len = length();
	TCHAR buff[len + 1];
	getCharacters(buff);
	_capitalizedString(buff);
	return stringWithCharacters(buff, len);
}

dString*
dString::lowercaseString() const {
	if (!_chars) throw new ExEmptyString;
	
	dUInteger len = length();
	TCHAR buff[len + 1];
	getCharacters(buff);
	_lowerCaseString(buff);
	return stringWithCharacters(buff, len);
}

dString*
dString::uppercaseString() const {
	if (!_chars) throw new ExEmptyString;
	
	dUInteger len = length();
	TCHAR buff[len + 1];
	getCharacters(buff);
	_upperCaseString(buff);
	return stringWithCharacters(buff, len);
}

void
dString::_setValueToRange(const TCHAR* chars, dRange rng, bool needsDeleteTail) {
	bool isDefined = false;
	if (kNotDefined == rng.length) {
		rng.length = (dUInteger)_tcslen(chars) + 1;
		isDefined = true;
	}
		
	if (!_chars) _chars = new CArrayT<TCHAR>(chars, rng.length);
	else _chars->setObjectsToRange(chars, rng, needsDeleteTail);
	
    if (!isDefined) _chars->addObject(_T('\0'));
}

dString*
dString::pathWithComponents(dArrayT<dString*>* components) {
	dString* result = string();
	dUInteger loc = 0;
	
	for (dUInteger i = 0, cnt = components->count(); i < cnt; ++i) {
		dString* str = components->objectAtIndex(i);
		dUInteger len = str->length();
		const TCHAR* buffer = str->chars();
		
		if (len >= 1) {
			if (IS_SLASH(buffer[len - 1])) --len;
			if (0 != len && IS_SLASH(buffer[0])) {
				++buffer;
				--len;
			}
		}
		
		if (0 != len) {
			result->_setValueToRange(buffer, dMakeRange(loc, len), true);
			result->_setValueToRange(SLASH_STR, dMakeRange(loc + len, 1), true);
			loc += len + 1;
		}
	}
	
	result->_chars->removeObjectsInRange(dMakeRange(result->_chars->count() - 2, 2));
	result->_chars->addObject(_T('\0'));
	
	return result;
}

dArrayT<dString*>*
dString::pathComponents() const {
	dArrayT<dString*>* array = CL_NEW(dArrayT<dString*>)->autorelease();
	dUInteger len = length();
	const TCHAR* buffer = chars();
	
	dUInteger e = 0;
	do {
		dUInteger b = e;
		while (e < len && !IS_SLASH(buffer[e]))
			++e;
		
		dobj_ptr<dString> str(alloc()->initWithCharacters(buffer + b, e - b));
		if (str->length())
			array->addObject(str.get());
		++e; // skip sepchar
	} while (e < len);
	
	return array;
}

bool
dString::isAbsolutePath() const {
	if (length() > 0) {
		const TCHAR firstCh = characterAtIndex(0);
		if (IS_SLASH(firstCh) || _T('~') == firstCh)
			return true;
		
		if (length() > 1) {
			if (characterAtIndex(1) == _T(':'))
				return true;
		}
	}
	
	return false;
}

dString*
dString::lastPathComponent() const {
	dUInteger len = length();
	const TCHAR* buffer = chars();
	
	if (len > 1 && IS_SLASH(buffer[len - 1]))
		--len;
	
	for (dInteger i = len; --i >= 0;) {
		if (IS_SLASH(buffer[i]) && i < len - 1)
			return stringWithCharacters(buffer + i + 1, len - i - 1);
	}
	
	return stringWithCharacters(buffer, len);
}

dString*
dString::pathExtension() const {
	dUInteger len = length();
	const TCHAR* buffer = chars();
	
	if (len > 0 && IS_SLASH(buffer[len - 1]))
		--len;
	
	for (dInteger i = len; --i >= 0;) {
		if(IS_SLASH(buffer[i]))
			return string();
		if (buffer[i] == _T('.'))
			return stringWithCharacters(buffer + i + 1, len - i - 1);
	}
	
	return string();
}

dString*
dString::stringByAppendingPathComponent(const dString* component) const {
	dUInteger len = length();
	
	if (0 == len) {
		if (component) return stringWithString(component);
		else return string();
	}
	
	dUInteger comLen = component->length();
	dUInteger totalLen = len + 1 + comLen;
	TCHAR characters[totalLen];
	
	getCharacters(characters);
	dInteger p = len;
	while (--p >= 0 && IS_SLASH(characters[p]));
	characters[++p] = SLASH;
	dUInteger q = 0;
	while (q < comLen && IS_SLASH(component->characterAtIndex(q))) ++q;
	component->getCharacters(characters + p + 1, dMakeRange(q, comLen - q));
	
	return stringWithCharacters(characters, p + 1 - q + comLen);
}

dString*
dString::stringByAppendingPathExtension(const dString* extension) const {
	CL_THROW_IF_NIL(extension);
	
	dUInteger len = length();
	if (len && characterAtIndex(len - 1) == SLASH)
		--len;
	
	dUInteger extLen = extension->length();
	dUInteger totalLen = len + extLen + 2;
	TCHAR characters[totalLen];
	
	getCharacters(characters);
	characters[len] = _T('.');
	extension->getCharacters(characters + len + 1);
	
	return stringWithCharacters(characters, totalLen);
}

dString*
dString::stringByDeletingLastPathComponent() const {
	dUInteger len = length();
	const TCHAR* buffer = chars();
	
	for (dInteger i = len; --i >= 0;) {
		if (IS_SLASH(buffer[i])) {
			if (0 == i) return &_kSlashString;
			else if (i + 1 < len) return stringWithCharacters(buffer, i);
		}
	}
    
	return string();
}

dString*
dString::stringByDeletingPathExtension() const {
	dUInteger len = length();
	const TCHAR* buffer = chars();
	
	if (len > 1 && IS_SLASH(buffer[len - 1]))
		--len;
	
	for (dUInteger i = len; --i > 0;) {
		if (IS_SLASH(buffer[i]) || IS_SLASH(buffer[i - 1])) break;
		else if (buffer[i] == _T('.')) return stringWithCharacters(buffer, i);
	}
	
	return stringWithCharacters(buffer, len);
}

dArrayT<dString*>*
dString::stringsByAppendingPaths(dArrayT<dString*>* paths) const {
	dArrayT<dString*>* result = CL_NEW(dArrayT<dString*>)->autorelease();
	
	for (dUInteger i = 0, cnt = paths->count(); i < cnt; ++i) {
		dString* str = paths->objectAtIndex(i);
		
		str = stringByAppendingPathComponent(str);
		result->addObject(str);
    }
	
	return result;
}


static char*
_wcharToLocale(const wchar_t* str) {
	char* ptr;
	size_t s;
	
	/* first arg == NULL means 'calculate needed space' */
	s = wcstombs(NULL, str, 0);
	
	/* a size of -1 means there are characters that could not
	 be converted to current locale */
	if (s == -1) return NULL;
	/* malloc the necessary space */
	if ((ptr = new char[s + 1]) == NULL)
		return NULL;
	
	/* really do it */
	wcstombs(ptr, str, s);
	/* ensure NULL-termination */
	ptr[s] = '\0';
	/* remember to SAFE_DELETE_ARRAY() ptr when done */
	return ptr;
}

static wchar_t*
_localeToWchar(const char* str) {
	wchar_t* ptr;
	size_t s;
	
	/* first arg == NULL means 'calculate needed space' */
	s = mbstowcs(NULL, str, 0);
	
	/* a size of -1 is triggered by an error in encoding; never
	 happen in ISO-8859-* locales, but possible in UTF-8 */
	if (s == -1) return NULL;
	/* malloc the necessary space */
	if ((ptr = new wchar_t[s + 1]) == NULL)
		return NULL;
	
	/* really do it */
	mbstowcs(ptr, str, s);
	/* ensure NULL-termination */
	ptr[s] = L'\0';
	/* remember to SAFE_DELETE_ARRAY() ptr when done */
	return ptr;
}

static TCHAR
_toUpperCase(TCHAR character) {
	if (character >= _T('a') && character <= _T('z'))
		return character - (_T('a') - _T('A'));
	return character;
}

static TCHAR
_toLowerCase(TCHAR character) {
	if (character >= _T('A') && character <= _T('Z'))
		return character + (_T('a') - _T('A'));
	return character;
}

static bool
_isNumChar(TCHAR character) {
	return (_T('0') <= character && character <= _T('9'));
}

static void
_upperCaseString(TCHAR* characters) {
	dUInteger i = 0;
	while (_T('\0') != characters[i]) {
		characters[i] = _toUpperCase(characters[i]);
        ++i;
    }
}

static void
_lowerCaseString(TCHAR* characters) {
	dUInteger i = 0;
	while (_T('\0') != characters[i]) {
		characters[i] = _toLowerCase(characters[i]);
        ++i;
    }
}

static void
_capitalizedString(TCHAR* characters) {
	TCHAR  prev = _T(' ');
	dUInteger i = 0;
	while (_T('\0') != characters[i]) {
		if (_T(' ') == prev) characters[i] = _toUpperCase(characters[i]);
		prev = characters[i];
        ++i;
	}
}

static dInteger
_ctoi(TCHAR* c, dUInteger* len) {
	dUInteger i = 0;
	dUInteger n = *len;
	TCHAR num[n + 1];
	
	while (i < n && _isNumChar(c[i])) {
		num[i] = c[i];
        ++i;
    }
	num[i] = _T('\0');
	*len = i;
	
	_stscanf(num, _T("%d"), &i);
	return i;
}

static void
_convertUCS2ToUTF32(ucs2char const* src, utf32char* dest, dUInteger chCount) {
	for (dUInteger i = 0; i < chCount; ++i) {
		*dest = *src;
        ++dest;
        ++src;
    }
}

