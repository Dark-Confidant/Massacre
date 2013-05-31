#pragma once

#include <mcr/io/IReader.h>

namespace mcr {
namespace io  {

class IFileReader: public IReader
{
public:
    virtual uint64      size() const     = 0;
    virtual uint64      tell() const     = 0;
    virtual void        seek(uint64 pos) = 0;
    virtual const char* filename() const = 0;
};

} // ns io
} // ns mcr
