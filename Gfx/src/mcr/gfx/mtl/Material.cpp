#include "Universe.h"
#include <mcr/gfx/mtl/Material.h>

#include <mcr/Log.h>
#include <mcr/gfx/mtl/Manager.h>
#include "mcr/gfx/GLState.h"
#include "ParamUploadFn.h"

namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Structors

Material::Material(Manager* mgr):
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

bool Material::_link()
{
    using namespace mtl;

    m_paramDefs.clear();
    m_buffers.clear();
    m_textures.clear();

    if (m_program)
        glDeleteProgram(m_program);

    m_program = glCreateProgram();

    for (auto shaderIt = m_shaders.shaders.begin(); shaderIt != m_shaders.shaders.end(); ++shaderIt)
        glAttachShader(m_program, (*shaderIt)->handle());

    glLinkProgram(m_program);

    GLint status;
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);

    if (!status)
    {
        GLint logLength;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);

        std::string logString((std::size_t) logLength, '\0');
        glGetProgramInfoLog(m_program, logLength, nullptr, &logString[0]);

        g_log->error("Material link failed: %s", logString.c_str());
        return false;
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

        ParamLayout layout;

        for (int i = 0; i < numUniforms; ++i)
        {
            GLsizei length;
            GLint   size;
            GLenum  type;
            glGetActiveUniform(m_program, i, longestUniName, &length, &size, &type, &name[0]);

            int loc = glGetUniformLocation(m_program, name.c_str());
            if (loc == -1)
                continue;
            
            switch (type)
            {
            case GL_SAMPLER_2D:
            case GL_SAMPLER_2D_ARRAY:
            case GL_SAMPLER_2D_SHADOW:
            case GL_SAMPLER_2D_ARRAY_SHADOW:
            case GL_SAMPLER_2D_MULTISAMPLE:
            case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
            case GL_SAMPLER_2D_RECT:
            case GL_SAMPLER_2D_RECT_SHADOW:
                {
                    auto unit = (GLint) m_mgr->requestTexUnit();

                    if (GLEW_EXT_direct_state_access)
                        glProgramUniform1i(m_program, loc, unit);
                    else
                        glUniform1i(loc, unit);

                    m_textures.push_back(std::make_pair(unit, nullptr));
                }
                continue;
            }

            switch (type)
            {
            case GL_FLOAT:             layout.addFloat(name.c_str());  break;
            case GL_FLOAT_VEC2:        layout.addVec2(name.c_str());   break;
            case GL_FLOAT_VEC3:        layout.addVec3(name.c_str());   break;
            case GL_FLOAT_VEC4:        layout.addVec4(name.c_str());   break;
            case GL_FLOAT_MAT4:        layout.addMat4(name.c_str());   break;
            case GL_DOUBLE:            layout.addDouble(name.c_str()); break;
            case GL_DOUBLE_VEC2:       layout.addDVec2(name.c_str());  break;
            case GL_DOUBLE_VEC3:       layout.addDVec3(name.c_str());  break;
            case GL_DOUBLE_VEC4:       layout.addDVec4(name.c_str());  break;
            case GL_DOUBLE_MAT4:       layout.addDMat4(name.c_str());  break;
            case GL_INT:               layout.addInt(name.c_str());    break;
            case GL_INT_VEC2:          layout.addIVec2(name.c_str());  break;
            case GL_INT_VEC3:          layout.addIVec3(name.c_str());  break;
            case GL_INT_VEC4:          layout.addIVec4(name.c_str());  break;
            case GL_UNSIGNED_INT:      layout.addUInt(name.c_str());   break;
            case GL_UNSIGNED_INT_VEC2: layout.addUVec2(name.c_str());  break;
            case GL_UNSIGNED_INT_VEC3: layout.addUVec3(name.c_str());  break;
            case GL_UNSIGNED_INT_VEC4: layout.addUVec4(name.c_str());  break;
            default: continue;
            }

            ParamDef pdef = {loc, nullptr};
            m_paramDefs.push_back(pdef);
        }

        setLayout(layout, false);
    } // if (numUniforms)

    GLint numUniformBlocks;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);

    if (numUniformBlocks)
    {
        GLint longestBlockName = 0;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &longestBlockName); // doesn't work with Intel HD Graphics (I)

        if (!longestBlockName)
            longestBlockName = 128;

        std::string name(longestBlockName, '\0');

        for (int i = 0; i < numUniformBlocks; ++i)
        {
            GLint length;
            glGetActiveUniformBlockName(m_program, i, longestBlockName, &length, &name[0]);

            if (length >= 6 && name.compare(std::size_t(length - 6), 6, "Layout") == 0)
                name[std::size_t(length - 6)] = '\0';

            if (auto buffer = m_mgr->paramBuffer(name.c_str()))
            {
                auto binding = m_mgr->requestParamBufferBinding();

                glUniformBlockBinding(m_program, (uint) i, binding);
                m_buffers.push_back(std::make_pair(binding, buffer));
            }
        }
    }

    return true;
}


//////////////////////////////////////////////////////////////////////////
// Parameter upload

void Material::syncParams()
{
    g_glState->setActiveProgram(m_program);

    for (std::size_t i = 0; i < m_paramDefs.size(); ++i)
    {
        auto& pdef = m_paramDefs[i];

        if (!pdef.dirtyData)
            continue;

        static auto uploadFnTable = GLEW_EXT_direct_state_access ? g_uploadFnTableDirect : g_uploadFnTableIndirect;

        uploadFnTable[layout().params[i].first](m_program, pdef.location, pdef.dirtyData);
        pdef.dirtyData = nullptr;
    }

    for (std::size_t i = 0; i < m_buffers.size(); ++i)
    {
        m_buffers[i].second->sync();
        g_glState->bindBufferBase(GL_UNIFORM_BUFFER, m_buffers[i].first, m_buffers[i].second->handle());
    }

    for (std::size_t i = 0; i < m_textures.size(); ++i)
        if (m_textures[i].second)
            g_glState->bindTexture(m_textures[i].first, m_textures[i].second->handle());
}

} // ns mtl
} // ns gfx
} // ns mcr
