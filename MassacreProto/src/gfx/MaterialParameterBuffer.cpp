#include "Universe.h"
#include <mcr/gfx/MaterialParameterBuffer.h>

#include "GLState.h"

namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// Helpers

static const GLenum g_bufferDrawUsageTable[] =
{
    GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW
};


//////////////////////////////////////////////////////////////////////////
// Structors

MaterialParameterBuffer::MaterialParameterBuffer(
    const char* name, const MaterialParameterBufferLayout& layout, Usage usage):
    m_name(name),
    m_layout(layout),
    m_bufferSize(0),
    m_usage(usage),
    m_dirty(true),
    m_handle()
{
    auto& params = layout.parameters;

    for (std::size_t i = 0; i < params.size(); ++i)
        m_bufferSize += params[i].first.sizeAligned();

    m_buffer = new byte[m_bufferSize];


    std::size_t offset = 0;

    for (std::size_t i = 0; i < params.size(); ++i)
    {
        Parameter param(params[i].first, &m_buffer[offset], &m_dirty);

        m_parameters.push_back(std::move(param));
        m_parametersByName[params[i].second] = i;

        offset += params[i].first.sizeAligned();
    }
}

MaterialParameterBuffer::~MaterialParameterBuffer()
{
    delete [] m_buffer;
}


//////////////////////////////////////////////////////////////////////////
// GL stuff

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
