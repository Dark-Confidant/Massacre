#include "Universe.h"
#include <mcr/gfx/mtl/Texture.h>

#include <algorithm>
#include <mcr/io/FileSystem.h>
#include "mcr/gfx/GLState.h"

namespace mcr {
namespace gfx {
namespace mtl {

struct TexHeaderTMP
{
    uint magic;
    int  width, height;
    uint fmt, size;
};

Texture::Texture(): m_hasAlpha(false)
{
    glGenTextures(1, &m_handle);
    g_glState->bindTexture(m_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_handle);
}

bool Texture::load(io::IReader* stream)
{
    TexHeaderTMP header;
    if (!stream || stream->read(header) != sizeof(header))
        return false;

    auto buffer = new byte[header.size];

    auto success = stream->read(buffer, header.size) == header.size;
    if (success)
    {
        m_size.set(header.width, header.height);

        switch (header.fmt)
        {
        case GL_COMPRESSED_RGBA:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            m_hasAlpha = true;
            break;
        default:
            m_hasAlpha = false;
        }

        g_glState->bindTexture(m_handle);

        glCompressedTexImage2D(GL_TEXTURE_2D, 0,
            header.fmt,
            header.width, header.height, 0,
            header.size, buffer);

        /*glTexImage2D(GL_TEXTURE_2D, 0,
            m_hasAlpha ? GL_COMPRESSED_RGBA : GL_COMPRESSED_RGB,
            m_size.x(), m_size.y(), 0, hasAlpha ? GL_RGBA : GL_RGB,
            GL_UNSIGNED_BYTE, buffer);*/

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    delete [] buffer;

    return success;
}

bool Texture::save(io::IWriter* writer) const
{
    if (!writer)
        return false;

    g_glState->bindTexture(m_handle);

    GLint fmt, bufSize;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &fmt);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &bufSize);

    TexHeaderTMP header =
    {
        0x20584554, // "TEX "
        m_size.x(), m_size.y(),
        (uint) fmt,
        (uint) bufSize
    };

    auto buffer = new byte[bufSize];
    glGetCompressedTexImage(GL_TEXTURE_2D, 0, buffer);

    std::size_t written = 0;

    written += writer->write(header);
    written += writer->write(buffer, (std::size_t) bufSize);

    delete [] buffer;

    return written == sizeof(header) + (std::size_t) bufSize;
}

} // ns mtl
} // ns gfx
} // ns mcr
