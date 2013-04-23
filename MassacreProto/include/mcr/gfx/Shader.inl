namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// Creators

inline rcptr<Shader> Shader::create(Type type)
{
    return new Shader(type);
}

inline rcptr<Shader> Shader::createFromFile(IFile* file)
{
    Type type;
    
    switch (Path::ext(file->filename())[0])
    {
    case 'v': type = Vertex;   break;
    case 'g': type = Geometry; break;
    case 'f': type = Fragment; break;
    default: return nullptr;
    }

    return createFromFile(type, file);
}

inline rcptr<Shader> Shader::createFromFile(Type type, IFile* file)
{
    auto result = new Shader(type);

    result->setSourceFromFile(file);
    return result;
}


//////////////////////////////////////////////////////////////////////////
// Source stuff

inline void Shader::setSourceFromFile(IFile* file, bool recompile /*= true*/)
{
    auto length = (size_t) file->size();
    auto contents = new char[length + 1];

    file->read(contents, length);
    contents[length] = 0;

    setSource(contents, recompile);

    delete [] contents;
}


//////////////////////////////////////////////////////////////////////////
// Constant stuff

inline int Shader::findConstant(const char* name) const
{
    for (int i = 0; i < (int) m_constants.size(); ++i)
        if (m_constants[i].name == name)
            return i;

    return -1;
}

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
