namespace mcr {
namespace gfx {

inline const Shader::ConstantInfo& Shader::constantInfo(int idx) const
{
    return m_constants[idx];
}

inline const std::string& Shader::constantValue(int idx) const
{
    if (idx == -1)
    {
        static const std::string empty;
        return empty;
    }

    return m_sources[m_constants[idx].stringIdx];
}

inline const std::string& Shader::constantValue(const char* name) const
{
    return constantValue(findConstant(name));
}

template <typename T>
inline void Shader::replaceConstant(int idx, const T& value)
{
    if (idx == -1)
        return;

    auto& info = m_constants[idx];

    GlslType type;
    auto str = glslValueString(glslClamp(value, info.minValue, info.maxValue), &type);

    if (type.length != info.type.length)
        return;

    replaceConstant(idx, str);
}

template <typename T>
inline void Shader::replaceConstant(const char* name, T value)
{
    replaceConstant(findConstant(name), value);
}

} // ns gfx
} // ns mcr
