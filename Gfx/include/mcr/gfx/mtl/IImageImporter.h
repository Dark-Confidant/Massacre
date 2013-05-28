#pragma once

#include <mcr/gfx/mtl/Image.h>

namespace mcr {
namespace gfx {
namespace mtl {

class IImageImporter: public RefCounted
{
public:
    virtual rcptr<Image> import(io::IFile* file) = 0;
};

} // ns mtl
} // ns gfx
} // ns mcr
