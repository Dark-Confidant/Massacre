namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Material structor

inline rcptr<Material> Material::create(Manager* mgr)
{
    return new Material(mgr);
}


//////////////////////////////////////////////////////////////////////////
// Accessors & mutators

inline const RenderState& Material::renderState() const
{
    return m_renderState;
}

inline uint Material::renderStateHash() const
{
    return m_renderStateHash;
}

inline void Material::setRenderState(const RenderState& state)
{
    m_renderState     = state;
    m_renderStateHash = state.hash();
}

inline const ShaderList& Material::shaders() const
{
    return m_shaders;
}

inline bool Material::setShaders(const ShaderList& shaders)
{
    m_shaders = shaders;
    return _link();
}

inline byte Material::numTextures() const
{
    return (byte) m_textures.size();
}

inline Texture* Material::texture(byte idx) const
{
    return m_textures[idx].second;
}

inline bool Material::setTexture(byte idx, Texture* tex)
{
    if (idx >= m_textures.size())
        return false;

    m_textures[idx].second = tex;
    return true;
}

inline int Material::passHint() const
{
    return m_passHint;
}

inline void Material::setPassHint(int pass)
{
    m_passHint = pass;
}

template <typename T>
inline void Material::set(T RenderState::* param, const T& val)
{
    m_renderState.*param = val;
    m_renderStateHash    = m_renderState.hash();
}

} // ns mtl
} // ns gfx
} // ns mcr
