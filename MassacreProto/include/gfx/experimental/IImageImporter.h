#pragma once

#include <gfx/Image.h>

namespace mcr {
namespace gfx {

class IImageImporter
{
public:
    virtual ~IImageImporter() {}
    virtual rcptr<Image> import(IFile* file) = 0;
};

} // ns gfx
} // ns mcr
