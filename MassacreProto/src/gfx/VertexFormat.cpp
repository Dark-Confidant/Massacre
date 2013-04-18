#include "Universe.h"
#include "gfx/VertexFormat.h"

#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "Debug.h"

namespace mcr {
namespace gfx {

VertexFormat::VertexFormat(const char* fmt):
    m_stride(0)
{
    enum { WantLengthOrSemantic, WantType } mode = WantLengthOrSemantic;

    GType type;
    uint len = 0;
    bool isUnsigned = false;
    char semantic = '\0';

    for (auto s = fmt;; ++s)
    {
        if (mode == WantLengthOrSemantic)
        {
            if (*s == '_') // empty attrib
            {
                addAttrib(GType::Float, 0);
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
            case 'B': case 'b': type = isUnsigned ? GType::Byte   : GType::SByte; break;
            case 'S': case 's': type = isUnsigned ? GType::UShort : GType::Short; break;
            case 'I': case 'i': type = isUnsigned ? GType::UInt   : GType::Int;   break;
            case 'F': case 'f': type = GType::Float;  break;
            case 'D': case 'd': type = GType::Double; break;
            default:
                if (isUnsigned)
                {
                    type = GType::UInt;
                    --s;
                }
            }

            if (type.valid())
            {
                addAttrib(type, len, semantic);
                type = GType::Unknown;
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
        case GType::SByte:  result += 'b';  break;
        case GType::Byte:   result += "ub"; break;
        case GType::Short:  result += 's';  break;
        case GType::UShort: result += "us"; break;
        case GType::Int:    result += 'i';  break;
        case GType::UInt:   result += "ui"; break;
        case GType::Float:  result += 'f';  break;
        case GType::Double: result += 'd';  break;
        default:
            result += '?';
            debug("Warning: unknown attrib type %u", attrib.type);
        }

        result += ' ';
    }

    return result;
}

} // ns gfx
} // ns mcr
