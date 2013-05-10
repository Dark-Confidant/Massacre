namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// Parameter

inline Material::Parameter::Parameter(MaterialParameterType type, uint program, int loc, UploadFn uploadFn):
IMaterialParameter(type),
    m_program(program), m_location(loc), m_uploadFn(uploadFn)
{
    m_data = new byte[type.size()];
}

inline Material::Parameter::~Parameter()
{
    delete [] m_data;
}

inline void Material::Parameter::sync()
{
    if (!m_dirty)
        return;

    m_uploadFn(m_program, m_location, m_data);
    m_dirty = false;
}

inline const void* Material::Parameter::mem() const
{
    return m_data;
}

inline void* Material::Parameter::mem()
{
    return m_data;
}


//////////////////////////////////////////////////////////////////////////
// Material structor

inline rcptr<Material> Material::create(MaterialManager* mgr)
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

inline const MaterialShaderSet& Material::shaders() const
{
    return m_shaders;
}

inline bool Material::setShaders(const MaterialShaderSet& shaders)
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

} // ns gfx
} // ns mcr
