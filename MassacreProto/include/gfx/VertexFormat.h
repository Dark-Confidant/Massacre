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

    VertexFormat(const char* fmt = "");

    static uint typeSize(uint type);

    uint addAttrib(uint type, uint length, char semantic, uint offset);
    uint addAttrib(uint type, uint length, char semantic = '\0');

    uint numAttribs() const { return (uint) m_attribs.size(); }

    const Attrib& attrib(uint i) const { return m_attribs[i]; }
    const Attrib& attribBySemantic(char semantic) const;

    uint stride() const { return m_stride; }

    void clear();

    std::string toString() const;

protected:
    std::vector<Attrib> m_attribs;
    uint m_stride;
};

} // ns gfx
} // ns mcr
