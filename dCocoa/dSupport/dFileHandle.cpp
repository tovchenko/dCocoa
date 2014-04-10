/*
** $Id: 2009/07/21, Taras Tovchenko $
** See Description in header
** See Copyright Notice in dCocoa.h
*/

#include "StdAfx.h"
#include "dFileHandle.h"

#include "dString.h"
#include "dData.h"

#if defined (IS_WIN)
	#include <io.h>
#endif


#define _FILE_OBJECT(result)        dFileHandle* result = CL_NEW(_self); if (!result) return nil; result->autorelease();



CL_CLASS_DEF(dFileHandle, dCocoa)

dFileHandle::ExCantOpenFile::ExCantOpenFile(const dString* path) {
    _path = path->copy();
}

dFileHandle::ExCantOpenFile::~ExCantOpenFile() {
    CL_RELEASE(_path);
}

dFileHandle::dFileHandle()
: _stream(nil) {
}

void
dFileHandle::dealloc() {
    closeFile();
    _super::dealloc();
}

dFileHandle*
dFileHandle::fileHandleForReadingAtPath(const dString* path) {
    return _openFile(path, _T("rb"));
}

dFileHandle*
dFileHandle::fileHandleForUpdatingAtPath(const dString* path) {
    return _openFile(path, _T("r+b"));
}

dFileHandle*
dFileHandle::fileHandleForWritingAtPath(const dString* path) {
    return _openFile(path, _T("wb"));
}

dData*
dFileHandle::readDataToEndOfFile() const {
    if (!_stream) throw new ExOperation;

    unsigned long curPos = offsetInFile();
    unsigned long length = seekToEndOfFile();
    seekToFileOffset(curPos);

    return _readData(curPos, length - curPos);
}

dData*
dFileHandle::readDataOfLength(dUInteger length) const {
    if (!_stream) throw new ExOperation;

    unsigned long curPos = offsetInFile();
    unsigned long endPos = seekToEndOfFile();
    length = (dUInteger)__min(length, endPos);
    seekToFileOffset(curPos);

    return _readData(curPos, length);
}

void
dFileHandle::writeData(const dData* data) {
	CL_THROW_IF_NIL(data);
    if (!_stream) throw new ExOperation;

    unsigned long len = data->length();
    if (len != std::fwrite(data->bytes(), 1, len, _stream))
        throw new ExOperation;
}

unsigned long
dFileHandle::offsetInFile() const {
    if (!_stream) throw new ExOperation;

    return std::ftell(_stream);
}

unsigned long
dFileHandle::seekToEndOfFile() const {
    if (!_stream) throw new ExOperation;

    std::fseek(_stream, 0, SEEK_END);
    return offsetInFile();
}

void
dFileHandle::seekToFileOffset(unsigned long offset) const {
    if (!_stream) throw new ExOperation;

    std::fseek(_stream, offset, SEEK_SET);
}

void
dFileHandle::closeFile() {
    if (_stream) {
        std::fclose(_stream);
        _stream = nil;
    }
}

void
dFileHandle::synchronizeFile() const {
    if (!_stream) throw new ExOperation;

    std::fflush(_stream);
}

void
dFileHandle::truncateFileAtOffset(unsigned long offset) {
    if (!_stream) throw new ExOperation;

    synchronizeFile();
	
#if defined(IS_WIN)
    if (_chsize(_fileno(_stream), offset))
#else
	if (ftruncate(fileno(_stream), offset))
#endif
        throw new ExOperation;

    seekToEndOfFile();
}

dFileHandle*
dFileHandle::_openFile(const dString* path, const TCHAR* mode) {
    _FILE_OBJECT(result)
	
#if defined(IS_WIN)
	const TCHAR* pathStr = path->chars();
    if (_tfopen_s(&result->_stream, pathStr, mode))
        throw new ExCantOpenFile(path);
#else
	if (!(result->_stream = fopen(dString::MultiByte(path), dString::MultiByte(mode))))
		throw new ExCantOpenFile(path);
#endif
    return result;
}

dData*
dFileHandle::_readData(unsigned long pos, unsigned long len) const {
    dByte* buffer = nil;
    try {
        buffer = new dByte[len];
        unsigned long cnt = std::fread(buffer, 1, len, _stream);
        if (cnt != len) throw new ExOperation;

        seekToFileOffset(pos);

        return dData::dataWithBytesNoCopy(buffer, (dUInteger)len, true);
    } catch (...) {
        SAFE_DELETE_ARRAY(buffer);
        throw;
    }
}
