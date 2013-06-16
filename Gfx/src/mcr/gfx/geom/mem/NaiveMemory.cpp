#include "Universe.h"
#include <mcr/gfx/geom/mem/NaiveMemory.h>

#include "mcr/gfx/GLState.h"

namespace mcr  {
namespace gfx  {
namespace geom {
namespace mem  {

NaiveMemory::Buffer::Buffer(const NaiveMemory* memory, std::size_t size):
    m_memory(memory), m_size(size)
{
    glGenBuffers(1, &m_vbo);
    g_glState->bindVertexArray(0);
    g_glState->bindBuffer(memory->m_target, m_vbo);
    glBufferData(memory->m_target, size, nullptr, memory->m_usage);
}

NaiveMemory::Buffer::~Buffer()
{
    glDeleteBuffers(1, &m_vbo);
}

void NaiveMemory::Buffer::read(std::size_t offset, std::size_t length, void* dst) const
{
    g_glState->bindVertexArray(0);
    g_glState->bindBuffer(m_memory->m_target, m_vbo);
    glGetBufferSubData(m_memory->m_target, (GLintptr) offset, (GLsizeiptr) length, dst);
}

void NaiveMemory::Buffer::write(std::size_t offset, std::size_t length, const void* src)
{
    g_glState->bindVertexArray(0);
    g_glState->bindBuffer(m_memory->m_target, m_vbo);
    glBufferSubData(m_memory->m_target, (GLintptr) offset, (GLsizeiptr) length, src);
}

NaiveMemory::NaiveMemory(Target target, Usage usage)
{
    m_target = target == Index ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
    m_usage  = usage  == Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
}

rcptr<IVideoBuffer> NaiveMemory::allocate(std::size_t size)
{
    return new Buffer(this, size);
}

} // ns mem
} // ns geom
} // ns gfx
} // ns mcr
