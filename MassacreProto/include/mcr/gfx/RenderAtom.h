#pragma once

#include <mcr/gfx/Material.h>

namespace mcr {
namespace gfx {

struct RenderAtom
{
    uint vao;
    uint start, size;
    rcptr<Material> material;
};

} // ns gfx
} // ns mcr