#pragma once

#include <mcr/io/IWriter.h>

namespace mcr {
namespace io  {

class IFileWriter: public IWriter
{
public:
    virtual uint64      tell() const     = 0;
    virtual void        seek(uint64 pos) = 0;
    virtual const char* filename() const = 0;
};

} // ns io
} // ns mcr
