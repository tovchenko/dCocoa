/*
** $Id: 2009/07/21, Taras Tovchenko $
** Wrapper for accessing open files or communications channels
** See Copyright Notice in dCocoa.h
*/

#ifndef _COCOA_DFILEHANDLE_H
#define _COCOA_DFILEHANDLE_H



BEGIN_NAMESPACE_CL
class dString;
class dData;

class DLL_PUBLIC dFileHandle : public dObject {
CL_CLASS_DECL(dFileHandle, dObject)
public:
    // exceptions
    class ExCantOpenFile : public CL::CException {
    public:
        ExCantOpenFile(const dString* path);
        ~ExCantOpenFile();

    protected:
        dString*    _path;
    };
    class ExOperation : public CL::CException { };
    // getting file handle
    static dFileHandle*         fileHandleForReadingAtPath(const dString* path);
    static dFileHandle*         fileHandleForUpdatingAtPath(const dString* path);
    static dFileHandle*         fileHandleForWritingAtPath(const dString* path);
    // reading
    dData*                      readDataToEndOfFile() const;
    dData*                      readDataOfLength(dUInteger length) const;
    // writing
    void                        writeData(const dData* data);
    // seeking
    unsigned long               offsetInFile() const;
    unsigned long               seekToEndOfFile() const;
    void                        seekToFileOffset(unsigned long offset) const;

    void                        closeFile();
    void                        synchronizeFile() const;
    void                        truncateFileAtOffset(unsigned long offset);

protected:
    dFileHandle();
    virtual void                dealloc();

    static dFileHandle*         _openFile(const dString* path, const TCHAR* mode);
    dData*                      _readData(unsigned long pos, unsigned long len) const;

    FILE*                       _stream;
};

END_NAMESPACE_CL

#endif // _COCOA_DFILEHANDLE_H