#include "Universe.h"
#include <mcr/gfx/Shader.h>

#include <mcr/Debug.h>

namespace mcr {
namespace gfx {

namespace {
uint g_shaderTypes[] =
{
    GL_VERTEX_SHADER,
    GL_GEOMETRY_SHADER,
    GL_FRAGMENT_SHADER
};
} // ns


//////////////////////////////////////////////////////////////////////////
// Structors

Shader::Shader(Type type):
    m_type(type),
    m_htype(g_shaderTypes[type]),
    m_preprocessor(),
    m_valid(false)
{
    m_handle = glCreateShader(m_htype);
}

Shader::~Shader()
{
    glDeleteShader(m_handle);
}


//////////////////////////////////////////////////////////////////////////
// GL interaction

bool Shader::compile()
{
    glCompileShader(m_handle);

    GLint status;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);

    m_valid = status == GL_TRUE;

    if (!m_valid)
        debug("Compilation failed:\n%s", log().c_str());

    return true;
}

std::string Shader::log() const
{
    GLint length;
    glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);

    std::string buf((size_t) length, '\0');
    glGetShaderInfoLog(m_handle, length, nullptr, &buf[0]);

    return buf;
}

void Shader::setSource(const char* source, bool recompile /*= true*/)
{
    m_sources.clear();

    if (!m_preprocessor || !m_preprocessor->preprocess(source, m_sources))
        m_sources.push_back(source);

    m_sourcePtrs   .resize(m_sources.size());
    m_sourceLengths.resize(m_sources.size());

    for (uint i = 0; i < m_sources.size(); ++i)
    {
        m_sourcePtrs   [i] =       m_sources[i].c_str();
        m_sourceLengths[i] = (int) m_sources[i].size();
    }

    glShaderSource(m_handle, m_sources.size(), &m_sourcePtrs[0], &m_sourceLengths[0]);

    if (recompile)
        compile();
}

} // ns gfx
} // ns mcr
