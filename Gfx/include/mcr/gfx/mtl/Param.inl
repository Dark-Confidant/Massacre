namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Type

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


//////////////////////////////////////////////////////////////////////////
// Parameter structors

inline IParam::IParam(ParamType type): m_type(type) {}
inline IParam::~IParam() {}


//////////////////////////////////////////////////////////////////////////
// Conversion

namespace detail {

template <ParamType::Type P, typename T>
struct ParamConverter
{
    static bool get(const void* mem, T& valueOut)
    {
        (void) mem;
        (void) valueOut;
        return false;
    }

    static bool set(void* mem, const T& value)
    {
        (void) mem;
        (void) value;
        return false;
    }
};

#define MCR_PARAM_EXPLICIT_CONVERSION(p, pt, t)         \
    template <>                                         \
    struct ParamConverter<ParamType::p, t>              \
    {                                                   \
        static bool get(const void* mem, t& valueOut)   \
        {                                               \
            valueOut = t(*static_cast<const pt*>(mem)); \
            return true;                                \
        }                                               \
                                                        \
        static bool set(void* mem, const t& value)      \
        {                                               \
            *static_cast<pt*>(mem) = pt(value);         \
            return true;                                \
        }                                               \
    }

#define MCR_PARAM_CONVERSION(p, t, t1, t2, t3)  \
    MCR_PARAM_EXPLICIT_CONVERSION(p, t, t);     \
    MCR_PARAM_EXPLICIT_CONVERSION(p, t, t1);    \
    MCR_PARAM_EXPLICIT_CONVERSION(p, t, t2);    \
    MCR_PARAM_EXPLICIT_CONVERSION(p, t, t3)

MCR_PARAM_CONVERSION(Float,  float,  double, int,    uint);
MCR_PARAM_CONVERSION(Double, double, float,  int,    uint);
MCR_PARAM_CONVERSION(Int,    int,    float,  double, uint);
MCR_PARAM_CONVERSION(UInt,   uint,   float,  double, int);
MCR_PARAM_CONVERSION(Vec2,   vec2,   dvec2,  ivec2,  uvec2);
MCR_PARAM_CONVERSION(DVec2,  dvec2,  vec2,   ivec2,  uvec2);
MCR_PARAM_CONVERSION(IVec2,  ivec2,  vec2,   dvec2,  uvec2);
MCR_PARAM_CONVERSION(UVec2,  uvec2,  vec2,   dvec2,  ivec2);
MCR_PARAM_CONVERSION(Vec3,   vec3,   dvec3,  ivec3,  uvec3);
MCR_PARAM_CONVERSION(DVec3,  dvec3,  vec3,   ivec3,  uvec3);
MCR_PARAM_CONVERSION(IVec3,  ivec3,  vec3,   dvec3,  uvec3);
MCR_PARAM_CONVERSION(UVec3,  uvec3,  vec3,   dvec3,  ivec3);
MCR_PARAM_CONVERSION(Vec4,   vec4,   dvec4,  ivec4,  uvec4);
MCR_PARAM_CONVERSION(DVec4,  dvec4,  vec4,   ivec4,  uvec4);
MCR_PARAM_CONVERSION(IVec4,  ivec4,  vec4,   dvec4,  uvec4);
MCR_PARAM_CONVERSION(UVec4,  uvec4,  vec4,   dvec4,  ivec4);

MCR_PARAM_EXPLICIT_CONVERSION(Mat4,  mat4,  mat4);
MCR_PARAM_EXPLICIT_CONVERSION(DMat4, dmat4, dmat4);

#undef MCR_PARAM_CONVERSION
#undef MCR_PARAM_EXPLICIT_CONVERSION

} // ns detail


//////////////////////////////////////////////////////////////////////////
// Accessors & mutators

inline ParamType IParam::type() const
{
    return m_type;
}

template <typename T>
inline bool IParam::get(T& valueOut) const
{
    switch (m_type)
    {
#define MCR_PARAM_CONVERT(p)    \
    case ParamType::p:          \
        return detail::ParamConverter<ParamType::p, T>::get(mem(), valueOut);

    MCR_PARAM_CONVERT(Float);
    MCR_PARAM_CONVERT(Double);
    MCR_PARAM_CONVERT(Int);
    MCR_PARAM_CONVERT(UInt);
    MCR_PARAM_CONVERT(Vec2);
    MCR_PARAM_CONVERT(DVec2);
    MCR_PARAM_CONVERT(IVec2);
    MCR_PARAM_CONVERT(UVec2);
    MCR_PARAM_CONVERT(Vec3);
    MCR_PARAM_CONVERT(DVec3);
    MCR_PARAM_CONVERT(IVec3);
    MCR_PARAM_CONVERT(UVec3);
    MCR_PARAM_CONVERT(Vec4);
    MCR_PARAM_CONVERT(DVec4);
    MCR_PARAM_CONVERT(IVec4);
    MCR_PARAM_CONVERT(UVec4);
    MCR_PARAM_CONVERT(Mat4);
    MCR_PARAM_CONVERT(DMat4);

#undef MCR_PARAM_CONVERT
    }
    return false;
}

template <typename T>
inline bool IParam::set(const T& value)
{
    bool result = false;

    switch (m_type)
    {
#define MCR_PARAM_CONVERT(p)                                                    \
    case ParamType::p:                                                          \
        result = detail::ParamConverter<ParamType::p, T>::set(mem(), value);    \
        break

    MCR_PARAM_CONVERT(Float);
    MCR_PARAM_CONVERT(Double);
    MCR_PARAM_CONVERT(Int);
    MCR_PARAM_CONVERT(UInt);
    MCR_PARAM_CONVERT(Vec2);
    MCR_PARAM_CONVERT(DVec2);
    MCR_PARAM_CONVERT(IVec2);
    MCR_PARAM_CONVERT(UVec2);
    MCR_PARAM_CONVERT(Vec3);
    MCR_PARAM_CONVERT(DVec3);
    MCR_PARAM_CONVERT(IVec3);
    MCR_PARAM_CONVERT(UVec3);
    MCR_PARAM_CONVERT(Vec4);
    MCR_PARAM_CONVERT(DVec4);
    MCR_PARAM_CONVERT(IVec4);
    MCR_PARAM_CONVERT(UVec4);
    MCR_PARAM_CONVERT(Mat4);
    MCR_PARAM_CONVERT(DMat4);

#undef MCR_PARAM_CONVERT
    }

    if (result)
        invalidate();

    return result;
}

template <typename T>
inline IParam& IParam::operator=(const T& rhs)
{
    set(rhs);
    return *this;
}

} // ns mtl
} // ns gfx
} // ns mcr
