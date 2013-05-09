#include "Universe.h"
#include <mcr/gfx/GBuffer.h>

#include "GLState.h"
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
    m_type(type),
    m_capacity(0)
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
    g_glState->bindBuffer(g_bufferTypeTable[m_type], m_handle);
    glBufferData(g_bufferTypeTable[m_type], size, nullptr, g_bufferUsageTable[usage]);

    m_capacity = size;
}

void GBuffer::init(const void* data, uint size, Usage usage)
{
    g_glState->bindBuffer(g_bufferTypeTable[m_type], m_handle);
    glBufferData(g_bufferTypeTable[m_type], size, data, g_bufferUsageTable[usage]);

    m_capacity = size;
}

uint GBuffer::capacity() const
{
    return m_capacity;
}

void* GBuffer::map(uint access)
{
    g_glState->bindBuffer(g_bufferTypeTable[m_type], m_handle);
    return glMapBuffer(g_bufferTypeTable[m_type], toLegacyGLMapAccess(g_mapAccessTable[access]));
}

void* GBuffer::map(uint offset, uint length, uint access)
{
    g_glState->bindBuffer(g_bufferTypeTable[m_type], m_handle);
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
    g_glState->bindBuffer(g_bufferTypeTable[m_type], m_handle);
    glUnmapBuffer(g_bufferTypeTable[m_type]);
}

} // ns gfx
} // ns mcr
