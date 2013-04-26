#pragma once

#include <mcr/gfx/Image.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

class IImageImporter: public RefCounted
{
public:
    virtual rcptr<Image> import(IFile* file) = 0;
};

} // ns experimental
} // ns gfx
} // ns mcr
