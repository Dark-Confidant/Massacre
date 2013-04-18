#include <Universe.h>
#include <gfx/GType.h>

namespace mcr   {
namespace gfx   {

ushort GType::toGLType() const
{
    static const ushort s_glEquiv[] =
    {
        GL_NONE,
        GL_BYTE,  GL_UNSIGNED_BYTE,
        GL_SHORT, GL_UNSIGNED_SHORT,
        GL_INT,   GL_UNSIGNED_INT,
        GL_FLOAT, GL_DOUBLE
    };

    return s_glEquiv[type];
}

GType GType::fromGLType(ushort glType)
{
    switch (glType)
    {
    case GL_BYTE:           return SByte;
    case GL_UNSIGNED_BYTE:  return Byte;
    case GL_SHORT:          return Short;
    case GL_UNSIGNED_SHORT: return UShort;
    case GL_INT:            return Int;
    case GL_UNSIGNED_INT:   return UInt;
    case GL_FLOAT:          return Float;
    case GL_DOUBLE:         return Double;
    default:                return Unknown;
    }
}

} // ns gfx
} // ns mcr
