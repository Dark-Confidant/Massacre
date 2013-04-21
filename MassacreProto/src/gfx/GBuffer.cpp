#include "Universe.h"
#include "gfx/GBuffer.h"

#include "gfx/Context.h"

using namespace mcr;
using namespace gfx;

namespace {

GLenum toLegacyAccess(GLbitfield access)
{
    switch (access & (GL_MAP_READ_BIT | GL_MAP_WRITE_BIT))
    {
    case GL_MAP_READ_BIT:  return GL_READ_ONLY;
    case GL_MAP_WRITE_BIT: return GL_WRITE_ONLY;
    default:               return GL_READ_WRITE;
    }
}

GLvoid* GLAPIENTRY glMapBufferRange_fallback(
    GLenum target,
    GLintptr offset,
    GLsizeiptr/* length*/,
    GLbitfield access)
{
    return (GLbyte*) glMapBuffer(target, toLegacyAccess(access)) + offset;
}

const GLenum g_typeTr[] =
{
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
    GL_UNIFORM_BUFFER,
    GL_TEXTURE_BUFFER,
    GL_TRANSFORM_FEEDBACK_BUFFER

    // TODO: support
    /*GL_ATOMIC_COUNTER_BUFFER,
    GL_COPY_READ_BUFFER,
    GL_COPY_WRITE_BUFFER,
    GL_DRAW_INDIRECT_BUFFER,
    GL_DISPATCH_INDIRECT_BUFFER,
    GL_PIXEL_PACK_BUFFER,
    GL_PIXEL_UNPACK_BUFFER,
    GL_SHADER_STORAGE_BUFFER*/
};

const GLenum g_usageTr[] =
{
    GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY,
    GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY,
    GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY
};

} // ns

GBuffer::GBuffer(Type type):
    m_type(type),
    m_target(g_typeTr[type])
{
    if (!glMapBufferRange)
        glMapBufferRange = glMapBufferRange_fallback;

    glGenBuffers(1, &m_handle);
}

GBuffer::~GBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

void GBuffer::init(uint size, Usage usage)
{
    Context::active().bindBuffer(this);
    glBufferData(m_target, size, nullptr, g_usageTr[usage]);
}

void GBuffer::init(const void* data, uint size, Usage usage)
{
    Context::active().bindBuffer(this);
    glBufferData(m_target, size, data, g_usageTr[usage]);
}

uint GBuffer::capacity() const
{
    GLint length;

    Context::active().bindBuffer(const_cast<GBuffer*>(this)); // HACK
    glGetBufferParameteriv(m_target, GL_BUFFER_SIZE, &length);

    return (uint) length;
}

void* GBuffer::map(uint access)
{
    Context::active().bindBuffer(this);
    return glMapBuffer(m_target, toLegacyAccess(access)); // HACK! access gets broken when corresponding GLenums change
}

void* GBuffer::map(uint offset, uint length, uint access)
{
    Context::active().bindBuffer(this);
    return glMapBufferRange(m_target, offset, length, access); // HACK! access gets broken when corresponding GLenums change
}

const void* GBuffer::map(uint access) const
{
    return const_cast<GBuffer*>(this)->map(access & ~Write);
}

const void* GBuffer::map(uint offset, uint length, uint access) const
{
    return const_cast<GBuffer*>(this)->map(offset, length, access & ~Write);
}

void GBuffer::unmap() const
{
    glUnmapBuffer(m_target);
}
