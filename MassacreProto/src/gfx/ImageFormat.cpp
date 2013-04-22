#include "Universe.h"
#include "gfx/ImageFormat.h"

namespace mcr {
namespace gfx {

ImageFormat::ImageFormat(): m_enum(GL_NONE), m_components(0) {}
ImageFormat::ImageFormat(uint glEnum, uint components): m_enum(glEnum), m_components(components) {}

ImageFormat ImageFormat::byGLEnum(uint glEnum)
{
    switch (glEnum)
    {
    case GL_RED: case GL_GREEN: case GL_BLUE: case GL_ALPHA:
    case GL_LUMINANCE: case GL_LUMINANCE_ALPHA:
        return ImageFormat(glEnum, 1);

    case GL_RGB:
        return ImageFormat(glEnum, 3);

    case GL_RGBA:
        return ImageFormat(glEnum, 4);

    default:
        return ImageFormat(glEnum, 0);
    }
}

ImageFormat ImageFormat::byComponents(uint components, bool allowCompression)
{
    switch (components)
    {
    case 1:  return ImageFormat(allowCompression ? GL_COMPRESSED_RED  : GL_RED,  components);
    case 3:  return ImageFormat(allowCompression ? GL_COMPRESSED_RGB  : GL_RGB,  components);
    case 4:  return ImageFormat(allowCompression ? GL_COMPRESSED_RGBA : GL_RGBA, components);
    default: return ImageFormat(GL_NONE, components);
    }
}

} // ns gfx
} // ns mcr
