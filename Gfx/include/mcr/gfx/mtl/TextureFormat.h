#pragma once

#include <mcr/Types.h>

namespace mcr {
namespace gfx {
namespace mtl {

struct TextureFormat
{
    enum Format
    {
        R8u,    RG8u,    RGB8u,    RGBA8u,      // Plain byte
        R8s,    RG8s,    RGB8s,    RGBA8s,      // Plain sbyte normalized
        R16u,   RG16u,   RGB16u,   RGBA16u,     // Plain ushort
        R16s,   RG16s,   RGB16s,   RGBA16s,     // Plain short normalized
        R16f,   RG16f,   RGB16f,   RGBA16f,     // Plain half
        R32f,   RG32f,   RGB32f,   RGBA32f,     // Plain float
        R8i,    RG8i,    RGB8i,    RGBA8i,      // Plain sbyte
        R8ui,   RG8ui,   RGB8ui,   RGBA8ui,     // Plain byte
        R16i,   RG16i,   RGB16i,   RGBA16i,     // Plain short
        R16ui,  RG16ui,  RGB16ui,  RGBA16ui,    // Plain ushort
        R32i,   RG32i,   RGB32i,   RGBA32i,     // Plain int
        R32ui,  RG32ui,  RGB32ui,  RGBA32ui,    // Plain uint
        NumPlainFormats,
        
        R_RGTC = NumPlainFormats,  RG_RGTC,     // Compressed RGTC
        Rs_RGTC,  RGs_RGTC,                     // Compressed normalized RGTC

        RGB_DXT1,  RGBA_DXT1,                   // Compressed DXT1
        RGBA_DXT3,                              // Compressed DXT3
        RGBA_DXT5,                              // Compressed DXT5

        RGBf_BPTC,                              // Compressed float BPTC (GL4)
        RGBuf_BPTC,                             // Compressed unsigned float BPTC (GL4)
        RGBA_BPTC,                              // Compressed normalized BPTC (GL4)
        NumFormats,
    }
    format;

    TextureFormat(Format fmt = NumFormats): format(fmt) {}
    explicit TextureFormat(uint fmt): format(Format(fmt)) {}

    operator Format()   const { return format; }

    bool isValid()      const { return format < NumFormats; }
    bool isPlain()      const { return format < NumPlainFormats; }
    bool isCompressed() const { return format >= NumPlainFormats && isValid(); }
};

} // ns mtl
} // ns gfx
} // ns mcr
