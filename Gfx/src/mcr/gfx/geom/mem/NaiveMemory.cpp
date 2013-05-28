#include "Universe.h"
#include <mcr/gfx/geom/mem/NaiveMemory.h>

#include "mcr/gfx/GLState.h"

namespace mcr  {
namespace gfx  {
namespace geom {
namespace mem  {

NaiveMemory::NaiveMemory(Target target, Usage usage)
{
    m_target = target == Index ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
    m_usage  = usage  == Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
}

VideoHandle NaiveMemory::allocate(std::size_t size)
{
    VideoHandle handle;

    handle.memory = this;
    handle.offset = 0;

    glGenBuffers(1, &handle.vbo);
    g_glState->bindVertexArray(0);
    g_glState->bindBuffer(m_target, handle.vbo);
    glBufferData(m_target, size, nullptr, m_usage);

    return handle;
}

void NaiveMemory::free(VideoHandle handle)
{
    glDeleteBuffers(1, &handle.vbo);
}

void NaiveMemory::read(VideoHandle src, std::size_t offset, std::size_t length, void* dst)
{
    g_glState->bindVertexArray(0);
    g_glState->bindBuffer(m_target, src.vbo);
    glGetBufferSubData(m_target, (GLintptr) offset, (GLsizeiptr) length, dst);
}

void NaiveMemory::write(const void* src, std::size_t length, VideoHandle dst, std::size_t offset)
{
    g_glState->bindVertexArray(0);
    g_glState->bindBuffer(m_target, dst.vbo);
    glBufferSubData(m_target, (GLintptr) offset, (GLsizeiptr) length, src);
}

} // ns mem
} // ns geom
} // ns gfx
} // ns mcr
