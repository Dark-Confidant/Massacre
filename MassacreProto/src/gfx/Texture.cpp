#include "Universe.h"
#include "gfx/Texture.h"

#include <algorithm>

using namespace mcr;
using namespace gfx;

rcptr<Texture> Texture::createFromFile(IFile* file, rcptr<Image>* imgOut)
{
    auto result = new Texture;

    result->m_filename = file->filename();

    auto slash = result->m_filename.find_last_of("/\\");
    if (slash != std::string::npos)
        result->m_filename = result->m_filename.substr(slash + 1);

    auto img = Image::createFromFile(file);
    if (imgOut)
        *imgOut = img;

    result->upload(img);

    return result;
}

rcptr<Texture> Texture::create()
{
    return new Texture;
}

Texture::Texture(): m_uploaded(false)
{
    glGenTextures(1, &m_handle);

    glBindTexture(GL_TEXTURE_2D, m_handle);
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

void Texture::load(IFile* file)
{
}

void Texture::upload(Image* img)
{
    m_filename   = img->filename();
    m_format     = img->format();
    m_size       = img->size();

    glBindTexture(GL_TEXTURE_2D, m_handle);

    glTexImage2D(GL_TEXTURE_2D, 0, m_format.openGLEnum(),
        m_size.x(), m_size.y(), 0, m_format.openGLEnum(),
        GL_UNSIGNED_BYTE, img->data());

    glGenerateMipmap(GL_TEXTURE_2D);

    m_uploaded = true;
}

void Texture::clear()
{
    glBindTexture(GL_TEXTURE_2D, m_handle);

    glTexImage2D(GL_TEXTURE_2D, 0, m_format.openGLEnum(),
        0, 0, 0, m_format.openGLEnum(),
        GL_UNSIGNED_BYTE, nullptr);

    m_uploaded = false;
}

rcptr<Image> Texture::download()
{
    glBindTexture(GL_TEXTURE_2D, m_handle);

    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  &m_size[0]);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_size[1]);

    auto img = Image::create(m_size, m_format);

    glGetTexImage(GL_TEXTURE_2D, 0, m_format.openGLEnum(), GL_UNSIGNED_BYTE, img->data());

    return img;
}
