/*
** $Id: 2009/05/06, Taras Tovchenko $
** Wrapper for represent generic types as dObjects.
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DVALUE_H
#define _COCOA_DVALUE_H



BEGIN_NAMESPACE_CL


class DLL_PUBLIC dValue : public dObject {
CL_CLASS_DECL(dValue, dObject)
// exceptions
class ExIncorrectType : public CL::CException { };

public:
    static dValue*              valueWithInt(dInteger intVal);
    static dValue*              valueWithFloat(float floatVal);
    static dValue*              valueWithDouble(double doubleVal);
    static dValue*              valueWithPtr(void* ptrVal);
	static dValue*				valueWithBool(bool boolVal);
	static dValue*				valueWithPoint(const dPoint& pointVal);
	static dValue*				valueWithSize(const dSize& sizeVal);
	static dValue*				valueWithRect(const dRect& rectVal);

    virtual dValue*             initWithInt(dInteger intVal);
    virtual dValue*             initWithFloat(float floatVal);
    virtual dValue*             initWithDouble(double doubleVal);
    virtual dValue*             initWithPtr(void* ptrVal);
	virtual dValue*				initWithBool(bool boolVal);
	virtual dValue*				initWithPoint(const dPoint& pointVal);
	virtual dValue*				initWithSize(const dSize& sizeVal);
	virtual dValue*				initWithRect(const dRect& rectVal);

    dInteger                    intValue() const;
    float                       floatValue() const;
    double                      doubleValue() const;
    void*                       ptrValue() const;
	bool						boolValue() const;
	dPoint						pointValue() const;
	dSize						sizeValue() const;
	dRect						rectValue() const;

    void                        setIntValue(dInteger intVal);
    void                        setFloatValue(float floatVal);
    void                        setDoubleValue(double doubleVal);
    void                        setPtrValue(void* ptrVal);
	void						setBoolValue(bool boolVal);
	void						setPointValue(const dPoint& pointVal);
	void						setSizeValue(const dSize& sizeVal);
	void						setRectValue(const dRect& rectVal);
	
    // hash & equal
    virtual dInteger            hash() const;
    virtual bool                isEqual(const dObject* otherObject) const;

protected:
    dValue();

    typedef enum {
        kTypeInt,
        kTypeFloat,
        kTypeDouble,
        kTypePtr,
		kTypeBool,
		kTypePoint,
		kTypeSize,
		kTypeRect
    } _Mode;

    dByte                       _buffer[16];
    _Mode                       _type;
};

END_NAMESPACE_CL

#endif // _COCOA_DVALUE_H