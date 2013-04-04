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
    default: return GL_READ_WRITE;
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

} // ns

GBuffer::GBuffer(uint type):
    m_type(type)
{
    if (!glMapBufferRange)
        glMapBufferRange = glMapBufferRange_fallback;

    glGenBuffers(1, &m_handle);
}

GBuffer::~GBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

void GBuffer::init(uint size, uint usage)
{
    Context::active().bindBuffer(this);
    glBufferData(m_type, size, nullptr, usage);
}

void GBuffer::init(const void* data, uint size, uint usage)
{
    Context::active().bindBuffer(this);
    glBufferData(m_type, size, data, usage);
}

uint GBuffer::capacity() const
{
    GLint length;

    Context::active().bindBuffer(const_cast<GBuffer*>(this)); // HACK
    glGetBufferParameteriv(m_type, GL_BUFFER_SIZE, &length);

    return (uint) length;
}

void* GBuffer::map(uint access)
{
    Context::active().bindBuffer(this);
    return glMapBuffer(m_type, toLegacyAccess(access));
}

void* GBuffer::map(uint offset, uint length, uint access)
{
    Context::active().bindBuffer(this);
    return glMapBufferRange(m_type, offset, length, access);
}

const void* GBuffer::map(uint access) const
{ return const_cast<GBuffer*>(this)->map(access & ~GL_MAP_WRITE_BIT); }

const void* GBuffer::map(uint offset, uint length, uint access) const
{ return const_cast<GBuffer*>(this)->map(offset, length, access & ~GL_MAP_WRITE_BIT); }

void GBuffer::unmap() const
{ glUnmapBuffer(m_type); }
