#pragma once

#include <vector>
#include <string>
#include <mcr/GfxExtern.h>
#include <mcr/Types.h>

namespace mcr  {
namespace gfx  {
namespace geom {

struct AttribType
{
    enum Type: uint
    {
        Unknown,
        SByte, Byte,
        Short, UShort,
        Int,   UInt,
        Float, Double,
    }
    type;

    AttribType(Type atype = Unknown): type(atype) {}
    explicit AttribType(uint atype): type(Type(atype)) {}

    operator Type() const { return type; }

    uint size() const;
    bool valid() const;
};

class VertexFormat
{
public:
    struct Attrib
    {
        uint location;
        AttribType type;
        uint length, offset;
        char semantic;
    };

    MCR_GFX_EXTERN VertexFormat(const char* fmt = "");

    uint addAttrib(AttribType type, uint length, char semantic, uint offset);
    uint addAttrib(AttribType type, uint length, char semantic = '\0');

    uint numAttribs() const;

    const Attrib& attrib(uint i) const;
    const Attrib* attribBySemantic(char semantic, uint nth = 0) const;

    uint stride() const;

    void clear();

    MCR_GFX_EXTERN std::string toString() const;

protected:
    std::vector<Attrib> m_attribs;
    uint m_stride;
};

} // ns geom
} // ns gfx
} // ns mcr

#include "VertexFormat.inl"
