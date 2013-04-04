#pragma once

#include "math/Vector.h"

namespace mcr {
namespace gfx {

struct RenderState
{
    RenderState();

    bool depthTest, depthWrite;
    int depthFunc;

    bool alphaTest;

    bool blend;
    ivec2 blendFunc;

    bool cullFace;
    bool polygonOffset;

    uint hash() const
    {
        return uint(depthTest)
            | (uint(depthWrite) << 2)
            | (uint(alphaTest) << 3)
            | (uint(blend) << 4)
            | (uint(cullFace) << 5)
            | (uint(polygonOffset) << 6)
            | (uint(depthFunc & 0x7) << 7)
            | (uint(blendFunc[0] & 0xF) << 10)
            | (uint(blendFunc[1] & 0xF) << 14);
    }
};

} // ns gfx
} // ns mcr
