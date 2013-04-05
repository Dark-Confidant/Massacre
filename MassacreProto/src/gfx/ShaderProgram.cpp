#include "Universe.h"
#include "gfx/ShaderProgram.h"

#include "gfx/Context.h"
#include "Debug.h"

namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// creators

rcptr<ShaderProgram> ShaderProgram::create()
{
    return new ShaderProgram;
}

rcptr<ShaderProgram> ShaderProgram::create(Shader* vert, Shader* frag)
{
    auto result = new ShaderProgram;

    result->attach(vert);
    result->attach(frag);

    result->link();

    return result;
}

rcptr<ShaderProgram> ShaderProgram::create(Shader* vert, Shader* geom, Shader* frag)
{
    auto result = new ShaderProgram;

    result->attach(vert);
    result->attach(geom);
    result->attach(frag);

    result->link();

    return result;
}

rcptr<ShaderProgram> ShaderProgram::createFromFiles(IFile* vert, IFile* frag)
{
    return create(
        Shader::createFromFile(Shader::Vertex, vert),
        Shader::createFromFile(Shader::Fragment, frag));
}

rcptr<ShaderProgram> ShaderProgram::createFromFiles(IFile* vert, IFile* geom, IFile* frag)
{
    return create(
        Shader::createFromFile(Shader::Vertex, vert),
        Shader::createFromFile(Shader::Geometry, geom),
        Shader::createFromFile(Shader::Fragment, frag));
}

//////////////////////////////////////////////////////////////////////////
// ctr/dtr

ShaderProgram::ShaderProgram():
    m_valid(false)
{
    m_handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_handle);
}

//////////////////////////////////////////////////////////////////////////
// attach/detach

byte ShaderProgram::attach(Shader* shader)
{
    auto result = numShaders();

    m_shaders.push_back(shader);
    glAttachShader(m_handle, shader->handle());

    return result;
}

void ShaderProgram::detach(Shader* shader)
{
    for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
        if (*it == shader)
        {
            glDetachShader(m_handle, shader->handle());
            m_shaders.erase(it);
        }
}

void ShaderProgram::detach(byte idx)
{
    glDetachShader(m_handle, m_shaders[idx]->handle());
    m_shaders.erase(m_shaders.begin() + idx);
}

//////////////////////////////////////////////////////////////////////////
// link

bool ShaderProgram::link()
{
    glLinkProgram(m_handle);

    GLint status;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &status);

    m_valid = status == GL_TRUE;
    
    if (!m_valid)
    {
        debug("Link failed:\n%s", log().c_str());
        return false;
    }

    Context::active().setActiveProgram(this);

    m_locations.clear();

    GLint numUniforms;
    glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &numUniforms);

    if (numUniforms)
    {
        GLint longestUniName;
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &longestUniName);

        auto nameBuf = new char[longestUniName];

        for (int i = 0; i < numUniforms; ++i)
        {
            GLsizei length;
            GLint   size;
            GLenum  type;
            glGetActiveUniform(m_handle, i, longestUniName, &length, &size, &type, nameBuf);

            int loc = glGetUniformLocation(m_handle, nameBuf);
            m_locations.insert(std::make_pair(std::move(std::string(nameBuf, (size_t) length)), loc));

            switch (type)
            {
            case GL_SAMPLER_1D:
            case GL_SAMPLER_2D:
            case GL_SAMPLER_3D:
            case GL_SAMPLER_CUBE:
            case GL_SAMPLER_1D_SHADOW:
            case GL_SAMPLER_2D_SHADOW:
            case GL_SAMPLER_1D_ARRAY:
            case GL_SAMPLER_2D_ARRAY:
            case GL_SAMPLER_1D_ARRAY_SHADOW:
            case GL_SAMPLER_2D_ARRAY_SHADOW:
            case GL_SAMPLER_2D_MULTISAMPLE:
            case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
            case GL_SAMPLER_CUBE_SHADOW:
            case GL_SAMPLER_BUFFER:
            case GL_SAMPLER_2D_RECT:
            case GL_SAMPLER_2D_RECT_SHADOW:
            case GL_INT_SAMPLER_1D:
            case GL_INT_SAMPLER_2D:
            case GL_INT_SAMPLER_3D:
            case GL_INT_SAMPLER_CUBE:
            case GL_INT_SAMPLER_1D_ARRAY:
            case GL_INT_SAMPLER_2D_ARRAY:
            case GL_INT_SAMPLER_2D_MULTISAMPLE:
            case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
            case GL_INT_SAMPLER_BUFFER:
            case GL_INT_SAMPLER_2D_RECT:
            case GL_UNSIGNED_INT_SAMPLER_1D:
            case GL_UNSIGNED_INT_SAMPLER_2D:
            case GL_UNSIGNED_INT_SAMPLER_3D:
            case GL_UNSIGNED_INT_SAMPLER_CUBE:
            case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
            case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
            case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
            case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
            case GL_UNSIGNED_INT_SAMPLER_BUFFER:
            case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
                {
                    SamplerInfo info;

                    info.name.assign(nameBuf, (size_t) length);

                    switch (type)
                    {
                    case GL_SAMPLER_1D:   info.target = Tex1D;   break;
                    case GL_SAMPLER_2D:   info.target = Tex2D;   break;
                    case GL_SAMPLER_3D:   info.target = Tex3D;   break;
                    case GL_SAMPLER_CUBE: info.target = TexCube; break;
                    default:              info.target = Tex2D;
                    }

                    info.unit = Context::active().allocTextureUnit();
                    glUniform1i(loc, (GLint) info.unit);

                    m_samplers.push_back(std::move(info));
                }
                break;
            }
        }

        delete [] nameBuf;
    }


    GLint numUniformBlocks;
    glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);

    m_blocks.resize((size_t) numUniformBlocks);

    if (numUniformBlocks)
    {
        GLint longestBlockName;
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &longestBlockName);

        auto nameBuf = new char[longestBlockName];

        for (int i = 0; i < numUniformBlocks; ++i)
        {
            GLint length;
            glGetActiveUniformBlockName(m_handle, i, longestBlockName, &length, nameBuf);
            m_blocks[i].name.assign(nameBuf, length);

            glGetActiveUniformBlockiv(m_handle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &m_blocks[i].size);

            glUniformBlockBinding(m_handle, i, i);
        }

        delete [] nameBuf;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
// log

std::string ShaderProgram::log() const
{
    GLint length;
    glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length);

    std::string buf((size_t) length, '\0');
    glGetProgramInfoLog(m_handle, length, nullptr, &buf[0]);

    return buf;
}

//////////////////////////////////////////////////////////////////////////
// uniform management

int ShaderProgram::findUniform(const char* name)
{
    return glGetUniformLocation(m_handle, name);
}

template <>
void ShaderProgram::Active::setUniform(int location, const float& value)
{
    glUniform1f(location, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const double& value)
{
    glUniform1d(location, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const int& value)
{
    glUniform1i(location, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const uint& value)
{
    glUniform1ui(location, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const vec2& value)
{
    glUniform2fv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const dvec2& value)
{
    glUniform2dv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const ivec2& value)
{
    glUniform2iv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const uvec2& value)
{
    glUniform2uiv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const vec3& value)
{
    glUniform3fv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const dvec3& value)
{
    glUniform3dv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const ivec3& value)
{
    glUniform3iv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const uvec3& value)
{
    glUniform3uiv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const vec4& value)
{
    glUniform4fv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const dvec4& value)
{
    glUniform4dv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const ivec4& value)
{
    glUniform4iv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const uvec4& value)
{
    glUniform4uiv(location, 1, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const mat4& value)
{
    glUniformMatrix4fv(location, 1, GL_TRUE, value);
}

template <>
void ShaderProgram::Active::setUniform(int location, const dmat4& value)
{
    glUniformMatrix4dv(location, 1, GL_TRUE, value);
}

} // ns gfx
} // ns mcr
