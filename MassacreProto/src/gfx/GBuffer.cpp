#include "Universe.h"
#include <gfx/GBuffer.h>

#include <gfx/Context.h>
#include "GLEnums.inl"

namespace mcr {
namespace gfx {

namespace {
GLvoid* GLAPIENTRY glMapBufferRange_fallback(
    GLenum target,
    GLintptr offset,
    GLsizeiptr length,
    GLbitfield access)
{
    (void) length;
    return (GLbyte*) glMapBuffer(target, toLegacyGLMapAccess(access)) + offset;
}
} // ns

GBuffer::GBuffer(Type type):
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

void GBuffer::init(uint size, Usage usage)
{
    Context::active().bindBuffer(this);
    glBufferData(g_bufferTypeTable[m_type], size, nullptr, g_bufferUsageTable[usage]);
}

void GBuffer::init(const void* data, uint size, Usage usage)
{
    Context::active().bindBuffer(this);
    glBufferData(g_bufferTypeTable[m_type], size, data, g_bufferUsageTable[usage]);
}

uint GBuffer::capacity() const
{
    GLint length;

    Context::active().bindBuffer(const_cast<GBuffer*>(this)); // HACK
    glGetBufferParameteriv(g_bufferTypeTable[m_type], GL_BUFFER_SIZE, &length);

    return (uint) length;
}

void* GBuffer::map(uint access)
{
    Context::active().bindBuffer(this);
    return glMapBuffer(g_bufferTypeTable[m_type], toLegacyGLMapAccess(g_mapAccessTable[access]));
}

void* GBuffer::map(uint offset, uint length, uint access)
{
    Context::active().bindBuffer(this);
    return glMapBufferRange(g_bufferTypeTable[m_type], offset, length, g_mapAccessTable[access]);
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
    glUnmapBuffer(g_bufferTypeTable[m_type]);
}

} // ns gfx
} // ns mcr
