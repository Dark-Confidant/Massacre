#pragma once

#include <mcr/Types.h>

namespace mcr {
namespace gfx {
namespace mtl {

struct TextureFormat
{
    enum Format
    {
        Depth16, Depth24, Depth32, Depth32f,
        Depth24Stencil8, Depth32fStencil8,
        NumSpecialFormats,

        R8u = NumSpecialFormats,
                RG8u,    RGB8u,    RGBA8u,      // Plain byte
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

    operator Format()       const { return format; }

    bool isValid()          const { return format < NumFormats; }
    bool isSpecial()        const { return format < NumSpecialFormats; }
    bool isPlain()          const { return format < NumPlainFormats && !isSpecial(); }
    bool isCompressed()     const { return format >= NumPlainFormats && isValid(); }
    byte numComponents()    const;
    byte bytesPerPixel()    const;
};


inline byte TextureFormat::numComponents() const
{
    if (isSpecial())
        return 1;

    if (isPlain())
        return byte(format - R8u) % 4 + 1;

    switch (format)
    {
    case R_RGTC: case Rs_RGTC: return 1;
    case RG_RGTC: case RGs_RGTC: return 2;
    case RGB_DXT1: case RGBf_BPTC: case RGBuf_BPTC: return 3;
    case RGBA_DXT1: case RGBA_DXT3: case RGBA_DXT5: case RGBA_BPTC: return 4;
    }

    return 0;
}

inline byte TextureFormat::bytesPerPixel() const
{
    static const byte bpp[NumPlainFormats] =
    {
        2, 3, 4, 4,
        4, 5,

        1, 2,  3,  4,   // Plain byte
        1, 2,  3,  4,   // Plain sbyte normalized
        2, 4,  6,  8,   // Plain ushort
        2, 4,  6,  8,   // Plain short normalized
        2, 4,  6,  8,   // Plain half
        4, 8, 12, 16,   // Plain float
        1, 2,  3,  4,   // Plain sbyte
        1, 2,  3,  4,   // Plain byte
        2, 4,  6,  8,   // Plain short
        2, 4,  6,  8,   // Plain ushort
        4, 8, 12, 16,   // Plain int
        4, 8, 12, 16,   // Plain uint
    };

    return isPlain() ? bpp[format] : 0;
}

} // ns mtl
} // ns gfx
} // ns mcr
