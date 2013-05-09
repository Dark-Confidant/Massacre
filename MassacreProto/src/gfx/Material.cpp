#include "Universe.h"
#include <mcr/gfx/Material.h>

#include <mcr/Debug.h>
#include <mcr/gfx/MaterialManager.h>
#include "MaterialParameterUploadFn.h"
#include "GLState.h"

namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// Parameter invalidation

void Material::Parameter::invalidate()
{
    if (GLEW_EXT_direct_state_access || m_program == g_glState->activeProgram())
        m_uploadFn(m_program, m_location, m_data);
    else
        m_dirty = true;
}


//////////////////////////////////////////////////////////////////////////
// Structors

Material::Material(MaterialManager* mgr):
    m_mgr(mgr),
    m_passHint(0),
    m_program()
{
    m_renderStateHash = m_renderState.hash();
}

Material::~Material()
{
    if (m_program)
        glDeleteProgram(m_program);
}


//////////////////////////////////////////////////////////////////////////
// Linkage

void Material::_link()
{
    if (m_program)
        glDeleteProgram(m_program);

    m_program = glCreateProgram();

    BOOST_FOREACH (auto& shader, m_shaders.shaders)
        glAttachShader(m_program, shader->handle());

    glLinkProgram(m_program);

    GLint status;
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);

    if (!status)
    {
        GLint logLength;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);

        std::string logString((std::size_t) logLength, '\0');
        glGetProgramInfoLog(m_program, logLength, nullptr, &logString[0]);

        debug("Material link failed: %s", logString.c_str());
        return;
    }

    if (!GLEW_EXT_direct_state_access)
        g_glState->setActiveProgram(m_program);

    GLint numUniforms;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &numUniforms);

    if (numUniforms)
    {
        GLint longestUniName;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &longestUniName);

        std::string name(longestUniName, '\0');

        for (int i = 0; i < numUniforms; ++i)
        {
            GLsizei length;
            GLint   size;
            GLenum  type;
            glGetActiveUniform(m_program, i, longestUniName, &length, &size, &type, &name[0]);

            int loc = glGetUniformLocation(m_program, name.c_str());
            if (loc == -1)
                continue;

            MaterialParameterType ptype = MaterialParameterType(~1);
            switch (type)
            {
                case GL_FLOAT:             ptype = MaterialParameterType::Float;  break;
                case GL_FLOAT_VEC2:        ptype = MaterialParameterType::Vec2;   break;
                case GL_FLOAT_VEC3:        ptype = MaterialParameterType::Vec3;   break;
                case GL_FLOAT_VEC4:        ptype = MaterialParameterType::Vec4;   break;
                case GL_FLOAT_MAT4:        ptype = MaterialParameterType::Mat4;   break;
                case GL_DOUBLE:            ptype = MaterialParameterType::Double; break;
                case GL_DOUBLE_VEC2:       ptype = MaterialParameterType::DVec2;  break;
                case GL_DOUBLE_VEC3:       ptype = MaterialParameterType::DVec3;  break;
                case GL_DOUBLE_VEC4:       ptype = MaterialParameterType::DVec4;  break;
                case GL_DOUBLE_MAT4:       ptype = MaterialParameterType::DMat4;  break;
                case GL_INT:               ptype = MaterialParameterType::Int;    break;
                case GL_INT_VEC2:          ptype = MaterialParameterType::IVec2;  break;
                case GL_INT_VEC3:          ptype = MaterialParameterType::IVec3;  break;
                case GL_INT_VEC4:          ptype = MaterialParameterType::IVec4;  break;
                case GL_UNSIGNED_INT:      ptype = MaterialParameterType::UInt;   break;
                case GL_UNSIGNED_INT_VEC2: ptype = MaterialParameterType::UVec2;  break;
                case GL_UNSIGNED_INT_VEC3: ptype = MaterialParameterType::UVec3;  break;
                case GL_UNSIGNED_INT_VEC4: ptype = MaterialParameterType::UVec4;  break;
            }

            if (ptype != MaterialParameterType(~1))
            {
                Parameter param(ptype, m_program, loc,
                    (GLEW_EXT_direct_state_access
                        ? g_uploadFnTableDirect
                        : g_uploadFnTableIndirect)[ptype]);

                m_params.push_back(std::move(param));
                continue;
            }

            switch (type)
            {
            case GL_SAMPLER_2D:
            case GL_SAMPLER_2D_SHADOW:
                {
                    auto unit = (GLint) m_mgr->requestTexUnit();

                    if (GLEW_EXT_direct_state_access)
                        glProgramUniform1i(m_program, loc, unit);
                    else
                        glUniform1i(loc, unit);

                    m_textures.push_back(std::make_pair(unit, nullptr));
                }
                break;
            }
        }
    } // if (numUniforms)

    GLint numUniformBlocks;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);

    if (numUniformBlocks)
    {
        GLint longestBlockName;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &longestBlockName);

        std::string name(longestBlockName, '\0');

        for (int i = 0; i < numUniformBlocks; ++i)
        {
            GLint length;
            glGetActiveUniformBlockName(m_program, i, longestBlockName, &length, &name[0]);

            if (length >= 6 && name.compare(std::size_t(length - 6), 6, "Layout") == 0)
                name[std::size_t(length - 6)] = '\0';

            if (auto buffer = m_mgr->parameterBuffer(name.c_str()))
            {
                auto binding = m_mgr->requestParameterBufferBinding();

                glUniformBlockBinding(m_program, (uint) i, binding);
                m_buffers.push_back(std::make_pair(binding, buffer));
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// Parameter upload

void Material::syncParameters()
{
    g_glState->setActiveProgram(m_program);

    for (std::size_t i = 0; i < m_params.size(); ++i)
        m_params[i].sync();

    for (std::size_t i = 0; i < m_buffers.size(); ++i)
        g_glState->bindBufferBase(GL_UNIFORM_BUFFER, m_buffers[i].first, m_buffers[i].second->handle());

    for (std::size_t i = 0; i < m_textures.size(); ++i)
        if (m_textures[i].second)
            g_glState->bindTexture(m_textures[i].first, m_textures[i].second->handle());
}

} // ns gfx
} // ns mcr
