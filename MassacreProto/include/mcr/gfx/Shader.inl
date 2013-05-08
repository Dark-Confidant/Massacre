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

inline void Shader::setSourceFromFile(IFile* file, bool recompile /*= true*/)
{
    auto length = (size_t) file->size();
    auto contents = new char[length + 1];

    file->read(contents, length);
    contents[length] = 0;

    setSource(contents, recompile);

    delete [] contents;
}

} // ns gfx
} // ns mcr
