/*
** $Id: 2009/07/06, Taras Tovchenko $
** OOP Wrapper for byte storage
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DDATA_H
#define _COCOA_DDATA_H


BEGIN_NAMESPACE_CL

class dString;

class DLL_PUBLIC dData : public dObject {
CL_CLASS_DECL(dData, dObject)
public:
	typedef enum _OwnMode {
		kCopy,
		kNoCopyNoDelete,
		kNoCopyWithDelete
	} OwnMode;
	
	static dData*				dataWithBytes(const void* bytes, dUInteger length);
	static dData*				dataWithBytesNoCopy(void* bytes, dUInteger length, bool deleteWhenDone = false);
	static dData*				dataWithData(const dData* data);
	static dData*				dataWithContentsOfFile(const dString* path);
	
	virtual dData*				initWithBytes(const void* bytes, dUInteger length);
	virtual dData*				initWithBytesNoCopy(void* bytes, dUInteger length, bool deleteWhenDone = false);
	virtual dData*				initWithData(const dData* data);
	virtual dData*				initWithContentsOfFile(const dString* path);
	
	void						setBytes(const void* bytes, dUInteger length, OwnMode mode);
	
	const void*					bytes() const;
	dUInteger					length() const;	
	// hash & equal
    virtual dInteger            hash() const;
    virtual bool                isEqual(const dObject* otherObject) const;
	// compressing
	dData*						compressedData() const;
	dData*						uncompressedData() const;
	
protected:
	dData();
	virtual void				dealloc();
	
	union {
		CArrayT<dByte>*			_bytesCopy;
		struct {
			dByte*				_bytesNoCopy;
			dUInteger			_length;
		};
	};
	OwnMode						_ownMode;
};

END_NAMESPACE_CL

#endif // _COCOA_DDATA_H