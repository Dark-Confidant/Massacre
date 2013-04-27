namespace mcr          {
namespace gfx          {
namespace experimental {

inline MaterialParameterType::MaterialParameterType(Type atype): type(atype) {}
inline MaterialParameterType::MaterialParameterType(uint atype): type(Type(atype)) {}

inline MaterialParameterType::operator Type() const
{
    return type;
}

inline std::size_t MaterialParameterType::size() const
{
    static const std::size_t s_sizes[] =
    {
        sizeof(float), sizeof(double), sizeof(int),   sizeof(uint),
        sizeof(vec2),  sizeof(dvec2),  sizeof(ivec2), sizeof(uvec2),
        sizeof(vec3),  sizeof(dvec3),  sizeof(ivec3), sizeof(uvec3),
        sizeof(vec4),  sizeof(dvec4),  sizeof(ivec4), sizeof(uvec4),
        sizeof(mat4),  sizeof(dmat4)
    };
    return s_sizes[type];
}



inline MaterialParameter::MaterialParameter(void* mem, UploadFn uploadFn, void* hint):
    m_mem(mem), m_hint(hint), m_uploadFn(uploadFn) {}

template <typename T>
inline const T& MaterialParameter::get() const
{
    return *static_cast<const T*>(m_mem);
}

template <typename T>
inline void MaterialParameter::set(const T& value)
{
    memcpy(m_mem, &value, sizeof(value));

    if (m_uploadFn)
        m_uploadFn(m_mem, m_hint);
}

template <typename T>
inline void MaterialParameter::operator=(const T& rhs)
{
    set(rhs);
}

} // ns experimental
} // ns gfx
} // ns mcr
