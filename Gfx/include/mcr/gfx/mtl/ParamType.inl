namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Parameter type

inline ParamType::ParamType(Type atype): type(atype) {}
inline ParamType::ParamType(uint atype): type(Type(atype)) {}

inline ParamType::operator Type() const
{
    return type;
}

inline std::size_t ParamType::size() const
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

inline std::size_t ParamType::sizeAligned() const
{
    static const std::size_t s_sizes[] =
    {
        sizeof(float), sizeof(double), sizeof(int),   sizeof(uint),
        sizeof(vec2),  sizeof(dvec2),  sizeof(ivec2), sizeof(uvec2),
        sizeof(vec4),  sizeof(dvec4),  sizeof(ivec4), sizeof(uvec4), // no typo
        sizeof(vec4),  sizeof(dvec4),  sizeof(ivec4), sizeof(uvec4),
        sizeof(mat4),  sizeof(dmat4)
    };
    return s_sizes[type];
}

} // ns mtl
} // ns gfx
} // ns mcr
