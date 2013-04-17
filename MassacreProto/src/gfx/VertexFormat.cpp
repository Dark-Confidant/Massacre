#include "Universe.h"
#include "gfx/VertexFormat.h"

#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "Debug.h"

namespace mcr {
namespace gfx {

MCR_EXTERN uint VertexFormat::typeSize(uint type)
{
    switch (type)
    {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
        return 1;

    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
        return 2;

    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FLOAT:
        return 4;

    case GL_DOUBLE:
        return 8;
    }
    return 0;
}


MCR_EXTERN VertexFormat::VertexFormat(const char* fmt):
    m_stride(0)
{
    enum { WantLengthOrSemantic, WantType } mode = WantLengthOrSemantic;

    uint len = 0, type = 0;
    bool isUnsigned = false;
    char semantic = '\0';

    for (auto s = fmt;; ++s)
    {
        if (mode == WantLengthOrSemantic)
        {
            if (*s == '_') // empty attrib
            {
                addAttrib(GL_FLOAT, 0);
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
            case 'B': case 'b': type = isUnsigned ? GL_UNSIGNED_BYTE  : GL_BYTE;  break;
            case 'S': case 's': type = isUnsigned ? GL_UNSIGNED_SHORT : GL_SHORT; break;
            case 'I': case 'i': type = isUnsigned ? GL_UNSIGNED_INT   : GL_INT;   break;
            case 'F': case 'f': type = GL_FLOAT;  break;
            case 'D': case 'd': type = GL_DOUBLE; break;
            default:
                if (isUnsigned)
                {
                    type = GL_UNSIGNED_INT;
                    --s;
                }
            }

            if (type)
            {
                addAttrib(type, len, semantic);
                type = 0;
                isUnsigned = false;
                semantic = '\0';
                mode = WantLengthOrSemantic;
            }
        }

        if (!*s)
            break;
    }
}


MCR_EXTERN std::string VertexFormat::toString() const
{
    std::string result;

    result.reserve(4 * m_attribs.size());

    for (uint i = 0; i < m_attribs.size(); ++i)
    {
        auto& attrib = m_attribs[i];

        result += attrib.semantic;
        result += boost::lexical_cast<std::string>(attrib.location);

        switch (attrib.type)
        {
        case GL_BYTE:           result += 'b';  break;
        case GL_UNSIGNED_BYTE:  result += "ub"; break;
        case GL_SHORT:          result += 's';  break;
        case GL_UNSIGNED_SHORT: result += "us"; break;
        case GL_INT:            result += 'i';  break;
        case GL_UNSIGNED_INT:   result += "ui"; break;
        case GL_FLOAT:          result += 'f';  break;
        case GL_DOUBLE:         result += 'd';  break;
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
