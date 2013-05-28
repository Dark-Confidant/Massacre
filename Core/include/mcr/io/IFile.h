#pragma once

#include <mcr/RefCounted.h>
#include <mcr/io/IReader.h>

namespace mcr {
namespace io  {

class FileSystem;

class IFile: public RefCounted, public IReader
{
public:
    //! File size in bytes, 0 if bad
    virtual uint64 size() const = 0;

    //! Seek to absolute position
    virtual void seek(uint64 pos) = 0;

    //! Tell current absolute position
    virtual uint64 tell() const = 0;

    //! File name
    virtual const char* filename() const = 0;

    //! Pointer to \c FileSystem this file belongs to
    virtual FileSystem* fs() const = 0;
};

} // ns io
} // ns mcr
