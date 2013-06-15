#include "Universe.h"
#include <mcr/gfx/geom/VertexFormat.h>

#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <mcr/Log.h>

namespace mcr  {
namespace gfx  {
namespace geom {

VertexFormat::VertexFormat(const char* fmt):
    m_stride(0)
{
    enum {WantLengthOrSemantic, WantType} mode = WantLengthOrSemantic;

    AttribType type;
    uint len = 0;
    bool isUnsigned = false;
    char semantic = '\0';

    for (auto s = fmt;; ++s)
    {
        if (mode == WantLengthOrSemantic)
        {
            if (*s == '_') // empty attrib
            {
                addAttrib(AttribType::Float, 0);
                continue;
            }

            if (*s >= '0' && *s <= '9') // length
            {
                len = *s - '0';
                mode = WantType;
                continue;
            }

            if ((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z')) // semantic
            {
                semantic = *s;
                continue;
            }
        }
        else // so mode != WantLengthOrSemantic
        {
            switch (*s)
            {
            case 'U': case 'u': isUnsigned = true; break;
            case 'B': case 'b': type = isUnsigned ? AttribType::Byte   : AttribType::SByte; break;
            case 'S': case 's': type = isUnsigned ? AttribType::UShort : AttribType::Short; break;
            case 'I': case 'i': type = isUnsigned ? AttribType::UInt   : AttribType::Int;   break;
            case 'F': case 'f': type = AttribType::Float;  break;
            case 'D': case 'd': type = AttribType::Double; break;
            default:
                if (isUnsigned)
                {
                    type = AttribType::UInt;
                    --s;
                }
            }

            if (type.valid())
            {
                addAttrib(type, len, semantic);
                type = AttribType::Unknown;
                isUnsigned = false;
                semantic = '\0';
                mode = WantLengthOrSemantic;
            }
        }

        if (!*s)
            break;
    }
}


std::string VertexFormat::toString() const
{
    std::string result;

    result.reserve(4 * m_attribs.size());

    for (uint i = 0; i < m_attribs.size(); ++i)
    {
        auto& attrib = m_attribs[i];

        result += attrib.semantic;
        result += boost::lexical_cast<std::string>(attrib.location);

        switch (attrib.type.type)
        {
        case AttribType::SByte:  result += 'b';  break;
        case AttribType::Byte:   result += "ub"; break;
        case AttribType::Short:  result += 's';  break;
        case AttribType::UShort: result += "us"; break;
        case AttribType::Int:    result += 'i';  break;
        case AttribType::UInt:   result += "ui"; break;
        case AttribType::Float:  result += 'f';  break;
        case AttribType::Double: result += 'd';  break;
        default:
            result += '?';
            g_log->warn("Unknown attribute type: %u", attrib.type);
        }

        result += ' ';
    }

    return result;
}

} // ns geom
} // ns gfx
} // ns mcr
