#pragma once

#include <vector>
#include <string>
#include "MassacreTypes.h"

namespace mcr {
namespace gfx {

class VertexFormat
{
public:
    struct Attrib
    {
        uint location, type, length, offset;
        char semantic;
    };

    MCR_EXTERN VertexFormat(const char* fmt = "");

    MCR_EXTERN static uint typeSize(uint type);

    uint addAttrib(uint type, uint length, char semantic, uint offset);
    uint addAttrib(uint type, uint length, char semantic = '\0');

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
