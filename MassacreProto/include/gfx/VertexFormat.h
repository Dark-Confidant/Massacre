#pragma once

#include <vector>
#include <string>
#include <gfx/GType.h>

namespace mcr {
namespace gfx {

class VertexFormat
{
public:
    struct Attrib
    {
        uint location;
        GType type;
        uint length, offset;
        char semantic;
    };

    MCR_EXTERN VertexFormat(const char* fmt = "");

    uint addAttrib(GType type, uint length, char semantic, uint offset);
    uint addAttrib(GType type, uint length, char semantic = '\0');

    uint numAttribs() const;

    const Attrib& attrib(uint i) const;
    const Attrib* attribBySemantic(char semantic) const;

    uint stride() const;

    void clear();

    MCR_EXTERN std::string toString() const;

protected:
    std::vector<Attrib> m_attribs;
    uint m_stride;
};

} // ns gfx
} // ns mcr

#include "VertexFormat.inl"
