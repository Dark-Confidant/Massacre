#pragma once

#include <mcr/Types.h>

namespace mcr {
namespace gfx {
namespace mtl {

struct TextureType
{
    enum Type
    {
        Tex1D,
        Tex2D,
        Tex3D,
        Tex1DArray,
        Tex2DArray,
        TexCubeMap,
        TexRect,
        Tex2DMS,
        Tex2DMSArray,
        TexBuffer,
        NumTypes
    }
    type;

    TextureType(Type atype = NumTypes): type(atype) {}
    explicit TextureType(uint atype): type(Type(atype)) {}

    operator Type()         const { return type; }

    bool isValid()          const { return type < NumTypes; }
    bool isArray()          const { return type == Tex1DArray || type == Tex2DArray || type == Tex2DMSArray; }
    bool isMultisampled()   const { return type == Tex2DMS || type == Tex2DMSArray; }
    bool supportsMipmaps()  const { return type <= TexCubeMap; }
    bool supportsFilters()  const { return type <= TexRect; }
    byte numDimensions()    const;
};


inline byte TextureType::numDimensions() const
{
    static const byte s_dims[NumTypes] = {1, 2, 3, 2, 3, 3, 2, 2, 3, 1};
    return s_dims[type];
}

} // ns mtl
} // ns gfx
} // ns mcr
