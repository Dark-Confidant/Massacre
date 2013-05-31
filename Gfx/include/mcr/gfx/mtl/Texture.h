#pragma once

#include <mcr/GfxExtern.h>
#include <mcr/math/Vector.h>
#include <mcr/io/IReader.h>
#include <mcr/io/IWriter.h>

namespace mcr {
namespace gfx {
namespace mtl {

struct TextureFormat
{
    enum DataType
    {
        Byte, SByte, Float, Int, UInt
    };

    DataType dataType;
    byte numComponents;
    bool compressed;

    TextureFormat(DataType adataType, byte anumComponents, bool acompressed):
        dataType(adataType), numComponents(anumComponents), compressed(acompressed) {}
};

struct ImageFormat
{
    enum Format
    {
        Red,
        RG,
        RGB,  BGR,
        RGBA, BGRA,
        Depth,
        Stencil,
        DepthStencil
    }
    format;

    ImageFormat(Format aformat): format(aformat) {}

    operator Format() const { return format; }
};


class Texture: public RefCounted
{
public:
    static rcptr<Texture>   create();

    uint                    handle() const;
    const ivec2&            size() const;
    bool                    hasAlpha() const;

    MCR_GFX_EXTERN bool     load(io::IReader* stream);
    MCR_GFX_EXTERN bool     save(io::IWriter* stream) const;

protected:
    MCR_GFX_EXTERN Texture();
    MCR_GFX_EXTERN ~Texture();

    uint    m_handle;
    ivec2   m_size;
    bool    m_hasAlpha;
};

} // ns mtl
} // ns gfx
} // ns mcr

#include "Texture.inl"
