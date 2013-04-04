#include "Universe.h"
#include "gfx/Shader.h"

#include "Debug.h"

using namespace mcr;
using namespace gfx;

namespace {

uint ShaderTypes[] =
{
    GL_VERTEX_SHADER,
    GL_GEOMETRY_SHADER,
    GL_FRAGMENT_SHADER
};

} // ns


rcptr<Shader> Shader::create(Type type)
{ return new Shader(type); }

rcptr<Shader> Shader::createFromFile(IFile* file)
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

rcptr<Shader> Shader::createFromFile(Type type, IFile* file)
{
    auto result = new Shader(type);

    result->setSourceFromFile(file);
    return result;
}


Shader::Shader(Type type):
    m_type(type),
    m_htype(ShaderTypes[type]),
    m_valid(false)
{
    m_handle = glCreateShader(m_htype);
}

Shader::~Shader()
{
    glDeleteShader(m_handle);
}


void Shader::setSource(const char* source, bool recompile /*= true*/)
{
    m_sources.clear();

    _preprocessSource(source);

    m_sourcePtrs   .resize(m_sources.size());
    m_sourceLengths.resize(m_sources.size());

    for (uint i = 0; i < m_sources.size(); ++i)
    {
        m_sourcePtrs   [i] =       m_sources[i].c_str();
        m_sourceLengths[i] = (int) m_sources[i].size();
    }

    _feedSource();

    if (recompile)
        compile();
}

void Shader::setSourceFromFile(IFile* file, bool recompile /*= true*/)
{
    auto length = (size_t) file->size();
    auto contents = new char[length + 1];

    file->read(contents, length);
    contents[length] = 0;

    setSource(contents, recompile);

    delete [] contents;
}

bool Shader::compile()
{
    glCompileShader(m_handle);

    GLint status;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);

    m_valid = status == GL_TRUE;

    if (!m_valid)
        debug("Compilation failed:\n%s", log().c_str());

    return true;
}

std::string Shader::log() const
{
    GLint length;
    glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);

    std::string buf((size_t) length, '\0');
    glGetShaderInfoLog(m_handle, length, nullptr, &buf[0]);

    return buf;
}

int Shader::findConstant(const char* name) const
{
    for (int i = 0; i < (int) m_constants.size(); ++i)
        if (m_constants[i].name == name)
            return i;

    return -1;
}

void Shader::replaceConstant(int idx, const std::string& value)
{
    if (idx == -1)
        return;

    auto& info   = m_constants[idx];
    auto& source = m_sources[info.stringIdx];

    source = value;

    m_sourcePtrs   [info.stringIdx] = source.c_str();
    m_sourceLengths[info.stringIdx] = source.size();

    _feedSource();
}


//////////////////////////////////////////////////////////////////////////
// Preprocessing stuff

namespace {

const char* nextline(const char* str)
{
    while (*str != '\r' && *str != '\n')
        ++str;

    if (*str == '\r' && str[1] == '\n')
        ++str;

    return ++str;
}

} // ns 

void Shader::_feedSource()
{
    glShaderSource(m_handle, m_sources.size(), &m_sourcePtrs[0], &m_sourceLengths[0]);
}

void Shader::_preprocessSource(const char* source)
{
    const char *first = source, *last = source;
    const char *cmdfirst, *cmdlast;

    ConstantInfo info;

    while (*last)
    {
        if (*last == '#' && _parseCommand(last, cmdfirst, cmdlast, info))
        {
            _addSourceString(first, cmdfirst);
            _addSourceString(cmdfirst, cmdlast);
            first = last = cmdlast;

            info.stringIdx = m_sources.size() - 1;
            m_constants.push_back(info);

            continue;
        }
        ++last;
    }

    _addSourceString(first, last);
}

void Shader::_addSourceString(const char* first, const char* last)
{
    if (first < last)
        m_sources.emplace_back(std::string(first, last));
}

bool Shader::_parseCommand(const char* str, const char*& cmdfirst, const char*& cmdlast, ConstantInfo& info)
{
    char desc[128];
    float minv, maxv;

    int parsed = sscanf(str, "#pragma%*[ \t]param%*[ \t]\"%[^\"]\"%*[ \t]%f%*[ \t]%f", desc, &minv, &maxv);
    if (parsed == 0)
        return false;

    str = nextline(str);

    bool limited = parsed == 3;
    char type[64], name[128], value[256];
    int  valBegin, valEnd;

    parsed = sscanf(str, " const%*[ \t\r\n]%s %s = %n%[^;]%n",
                    type, name, &valBegin, value, &valEnd);

    if (parsed != 3)
        return false;

    info.minValue     = minv;
    info.maxValue     = maxv;
    info.defaultValue = value;
    info.name         = name;
    info.description  = desc;

    _parseType(type, info.type);

    cmdfirst = str + valBegin;
    cmdlast  = str + valEnd;
    return true;
}

void Shader::_parseType(const char* str, GlslType& type)
{
    type.glTypeEnum = GL_NONE;

    switch (*str)
    {
    case 'f': type.glTypeEnum = GL_FLOAT; type.length = 1;  return;
    case 'm': type.glTypeEnum = GL_FLOAT; type.length = 16; return;
    case 'v': type.glTypeEnum = GL_FLOAT;                   break;
    case 'd': type.glTypeEnum = GL_DOUBLE;       ++str;     break;
    case 'b': type.glTypeEnum = GL_BOOL;         ++str;     break;
    case 'i': type.glTypeEnum = GL_INT;          ++str;     break;
    case 'u': type.glTypeEnum = GL_UNSIGNED_INT; ++str;     break;
    }

    switch (*str)
    {
    case 'v': type.length = *(str + 3) - '0'; return;
    case 'm': type.length = 16;               return;
    default:  type.length = 1;
    }
}
