#pragma once

#include <vector>
#include <boost/functional/hash.hpp>
#include "RenderState.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace mcr {
namespace gfx {

class Material: public RefCounted
{
public:
    static rcptr<Material> create(byte numTex = 1);


    const RenderState& renderState() const { return m_renderState; }
    uint renderStateHash() const { return m_renderStateHash; }

    void setRenderState(const RenderState& state)
    {
        m_renderState     = state;
        m_renderStateHash = state.hash();
    }

    template <typename T>
    void set(T RenderState::* param, const T& val)
    {
        m_renderState.*param = val;
        m_renderStateHash    = m_renderState.hash();
    }


    ShaderProgram* program() const { return m_shader; }

    void initProgram(Shader* vertShader, Shader* fragShader)
    {
        m_shader = ShaderProgram::create(vertShader, fragShader);
    }


    byte numTextures() const
    {
        return (byte) m_textures.size();
    }

    void setNumTextures(byte num)
    {
        m_textures.resize(num, nullptr);
    }

    Texture* texture(byte idx) const
    {
        return m_textures[idx];
    }

    void setTexture(byte idx, Texture* tex)
    {
        m_textures[idx] = tex;
    }


    int passHint() const
    {
        return m_passHint;
    }

    void setPassHint(int pass)
    {
        m_passHint = pass;
    }

protected:
    Material();
    ~Material();

    RenderState                 m_renderState;
    uint                        m_renderStateHash;
    rcptr<ShaderProgram>        m_shader;
    std::vector<rcptr<Texture>> m_textures;

    int m_passHint;
};

} // ns gfx
} // ns mcr
