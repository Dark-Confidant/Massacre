#pragma once

#include <mcr/gfx/Image.h>

namespace mcr          {
namespace gfx          {

class IImageImporter: public RefCounted
{
public:
    virtual rcptr<Image> import(IFile* file) = 0;
};

} // ns gfx
} // ns mcr
