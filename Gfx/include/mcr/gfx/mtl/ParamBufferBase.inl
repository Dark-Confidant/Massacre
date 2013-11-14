namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Conversion helpers

namespace detail {

template <typename T>        struct IsCompound                     { enum { Value = false }; };
template <typename T, int n> struct IsCompound<math::Vector<T, n>> { enum { Value = true  }; };
template <typename T>        struct IsCompound<math::Matrix4x4<T>> { enum { Value = true  }; };

template <typename Src, typename Dst>
inline bool convert(const Src& src, Dst& dst, char(*)[IsCompound<Src>::Value || IsCompound<Dst>::Value] = 0)
{
    return false;
}

template <typename Src, typename Dst>
inline bool convert(const Src& src, Dst& dst, char(*)[!IsCompound<Src>::Value && !IsCompound<Dst>::Value] = 0)
{
    dst = static_cast<Dst>(src);
    return true;
}

template <typename SrcT, typename DstT, int n>
inline bool convert(const math::Vector<SrcT, n>& src, math::Vector<DstT, n>& dst)
{
    dst = static_cast<math::Vector<DstT, n>>(src);
    return true;
}

template <typename SrcT, typename DstT>
inline bool convert(const math::Matrix4x4<SrcT>& src, math::Matrix4x4<DstT>& dst)
{
    for (std::size_t i = 0; i < 16; ++i)
        dst[i] = static_cast<DstT>(src[i]);

    return true;
}

} // ns detail


//////////////////////////////////////////////////////////////////////////
// Public members

inline int ParamBufferBase::numParams() const
{
    return (int) m_params.size();
}

inline const std::string& ParamBufferBase::paramName(int index) const
{
    static const std::string s_empty;
    return index >= 0 && (std::size_t) index < m_params.size() ? m_layout.params[index].second : s_empty;
}

inline ParamType ParamBufferBase::paramType(int index) const
{
    return index >= 0 && (std::size_t) index < m_params.size() ? m_layout.params[index].first : ParamType(ParamType::Float);
}

inline int ParamBufferBase::findParam(const std::string& pname) const
{
    auto it = m_paramsByName.find(pname);
    return it != m_paramsByName.end() ? (int) it->second : -1;
}

template <typename T>
inline bool ParamBufferBase::setParam(const std::string& pname, const T& value)
{
    return setParam(findParam(pname), value);
}

template <typename T>
inline bool ParamBufferBase::getParam(const std::string& pname, T& valueOut) const
{
    return setParam(findParam(pname), valueOut);
}

template <typename T>
inline bool ParamBufferBase::getParam(int index, T& valueOut) const
{
    using detail::convert;

    if (index < 0 || (std::size_t) index >= m_params.size())
        return false;

    switch(m_layout.params[index].first)
    {
    case ParamType::Float:  return convert(*(const float*)  m_params[index], valueOut);
    case ParamType::Double: return convert(*(const double*) m_params[index], valueOut);
    case ParamType::Int:    return convert(*(const int*)    m_params[index], valueOut);
    case ParamType::UInt:   return convert(*(const uint*)   m_params[index], valueOut);
    case ParamType::Vec2:   return convert(*(const vec2*)   m_params[index], valueOut);
    case ParamType::DVec2:  return convert(*(const dvec2*)  m_params[index], valueOut);
    case ParamType::IVec2:  return convert(*(const ivec2*)  m_params[index], valueOut);
    case ParamType::UVec2:  return convert(*(const uvec2*)  m_params[index], valueOut);
    case ParamType::Vec3:   return convert(*(const vec3*)   m_params[index], valueOut);
    case ParamType::DVec3:  return convert(*(const dvec3*)  m_params[index], valueOut);
    case ParamType::IVec3:  return convert(*(const ivec3*)  m_params[index], valueOut);
    case ParamType::UVec3:  return convert(*(const uvec3*)  m_params[index], valueOut);
    case ParamType::Vec4:   return convert(*(const vec4*)   m_params[index], valueOut);
    case ParamType::DVec4:  return convert(*(const dvec4*)  m_params[index], valueOut);
    case ParamType::IVec4:  return convert(*(const ivec4*)  m_params[index], valueOut);
    case ParamType::UVec4:  return convert(*(const uvec4*)  m_params[index], valueOut);
    case ParamType::Mat4:   return convert(*(const mat4*)   m_params[index], valueOut);
    case ParamType::DMat4:  return convert(*(const dmat4*)  m_params[index], valueOut);
    }
    return false;
}

template <typename T>
inline bool ParamBufferBase::setParam(int index, const T& value)
{
    using detail::convert;

    if (index < 0 || (std::size_t) index >= m_params.size())
        return false;

    bool success = false;

    switch(m_layout.params[index].first)
    {
    case ParamType::Float:  success = convert(value, *(float*)  m_params[index]); break;
    case ParamType::Double: success = convert(value, *(double*) m_params[index]); break;
    case ParamType::Int:    success = convert(value, *(int*)    m_params[index]); break;
    case ParamType::UInt:   success = convert(value, *(uint*)   m_params[index]); break;
    case ParamType::Vec2:   success = convert(value, *(vec2*)   m_params[index]); break;
    case ParamType::DVec2:  success = convert(value, *(dvec2*)  m_params[index]); break;
    case ParamType::IVec2:  success = convert(value, *(ivec2*)  m_params[index]); break;
    case ParamType::UVec2:  success = convert(value, *(uvec2*)  m_params[index]); break;
    case ParamType::Vec3:   success = convert(value, *(vec3*)   m_params[index]); break;
    case ParamType::DVec3:  success = convert(value, *(dvec3*)  m_params[index]); break;
    case ParamType::IVec3:  success = convert(value, *(ivec3*)  m_params[index]); break;
    case ParamType::UVec3:  success = convert(value, *(uvec3*)  m_params[index]); break;
    case ParamType::Vec4:   success = convert(value, *(vec4*)   m_params[index]); break;
    case ParamType::DVec4:  success = convert(value, *(dvec4*)  m_params[index]); break;
    case ParamType::IVec4:  success = convert(value, *(ivec4*)  m_params[index]); break;
    case ParamType::UVec4:  success = convert(value, *(uvec4*)  m_params[index]); break;
    case ParamType::Mat4:   success = convert(value, *(mat4*)   m_params[index]); break;
    case ParamType::DMat4:  success = convert(value, *(dmat4*)  m_params[index]); break;
    }

    if (success)
        onInvalidateParam(index, m_params[index]);

    return success;
}


//////////////////////////////////////////////////////////////////////////
// Protected members

inline const ParamLayout& ParamBufferBase::layout() const
{
    return m_layout;
}

inline void ParamBufferBase::setLayout(const ParamLayout& layout, bool aligned /*= true*/)
{
    m_layout = layout;

    m_params.resize(m_layout.params.size());
    m_paramsByName.clear();

    if (m_layout.params.empty())
        return;

    m_data.resize(aligned ? m_layout.totalSizeAligned() : m_layout.totalSize());

    for (std::size_t i = 0, offset = 0; i < m_layout.params.size(); ++i)
    {
        auto& pdef = m_layout.params[i];

        m_params[i] = &m_data[offset];
        m_paramsByName[pdef.second] = i;

        offset += aligned ? pdef.first.sizeAligned() : pdef.first.size();
    }
}

inline const std::vector<byte>& ParamBufferBase::data() const
{
    return m_data;
}

} // ns mtl
} // ns gfx
} // ns mcr
