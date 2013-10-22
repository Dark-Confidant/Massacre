#include "Universe.h"
#include <mcr/gfx/mtl/ParamBuffer.h>

#include "mcr/gfx/GLState.h"

namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Helpers

static const GLenum g_bufferDrawUsageTable[] =
{
    GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW
};


//////////////////////////////////////////////////////////////////////////
// Structors

ParamBuffer::ParamBuffer(const char* name, const ParamLayout& layout, Usage usage):
    m_name(name), m_usage(usage), m_dirty(true), m_handle()
{
    setLayout(layout);
}

ParamBuffer::~ParamBuffer() {}


//////////////////////////////////////////////////////////////////////////
// GL stuff

void ParamBuffer::init()
{
    glGenBuffers(1, &m_handle);
}

void ParamBuffer::sync()
{
    if (!m_dirty || !m_handle || !numParams())
        return;

    if (GLEW_EXT_direct_state_access)
        glNamedBufferDataEXT(m_handle, data().size(), &data()[0], g_bufferDrawUsageTable[m_usage]);
    else
    {
        g_glState->bindBuffer(GL_UNIFORM_BUFFER, m_handle);
        glBufferData(GL_UNIFORM_BUFFER, data().size(), &data()[0], g_bufferDrawUsageTable[m_usage]);
    }

    m_dirty = false;
}

} // ns mtl
} // ns gfx
} // ns mcr
