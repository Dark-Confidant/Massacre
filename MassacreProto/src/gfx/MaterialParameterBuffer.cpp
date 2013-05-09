#include "Universe.h"
#include <mcr/gfx/MaterialParameterBuffer.h>

#include "GLState.h"

namespace mcr {
namespace gfx {

static const GLenum g_bufferDrawUsageTable[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};

void MaterialParameterBuffer::init()
{
    glGenBuffers(1, &m_handle);
}

void MaterialParameterBuffer::sync()
{
    if (!m_dirty || !m_handle)
        return;

    if (GLEW_EXT_direct_state_access)
        glNamedBufferDataEXT(m_handle, m_bufferSize, m_buffer, g_bufferDrawUsageTable[m_usage]);
    else
    {
        g_glState->bindBuffer(GL_UNIFORM_BUFFER, m_handle);
        glBufferData(GL_UNIFORM_BUFFER, m_bufferSize, m_buffer, g_bufferDrawUsageTable[m_usage]);
    }

    m_dirty = false;
}

} // ns gfx
} // ns mcr
