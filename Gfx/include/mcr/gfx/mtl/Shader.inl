namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Creators

inline rcptr<Shader> Shader::create(Type type)
{
    return new Shader(type);
}


//////////////////////////////////////////////////////////////////////////
// Accessors & mutators

inline IShaderPreprocessor* Shader::preprocessor() const
{
    return m_preprocessor;
}

inline void Shader::setPreprocessor(IShaderPreprocessor* pp)
{
    m_preprocessor = pp;
}


//////////////////////////////////////////////////////////////////////////
// Source stuff

inline void Shader::setSourceFromStream(io::IReader* stream, bool recompile)
{
    std::string buffer;
    stream->readString0(buffer);

    setSource(buffer.c_str(), recompile);
}

} // ns mtl
} // ns gfx
} // ns mcr
