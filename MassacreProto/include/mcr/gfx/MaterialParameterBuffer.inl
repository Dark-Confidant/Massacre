namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// Parameter

inline MaterialParameterBuffer::Parameter::Parameter(MaterialParameterType type, void* mem, bool* dirty):
IMaterialParameter(type),
    m_mem(mem), m_dirty(dirty) {}

inline const void* MaterialParameterBuffer::Parameter::mem() const
{
    return m_mem;
}

inline void* MaterialParameterBuffer::Parameter::mem()
{
    return m_mem;
}

inline void MaterialParameterBuffer::Parameter::invalidate()
{
    *m_dirty = true;
}


//////////////////////////////////////////////////////////////////////////
// Buffer structors

inline MaterialParameterBuffer::MaterialParameterBuffer(
    const char* name, const MaterialParameterBufferLayout& layout, Usage usage):
    m_name(name),
    m_layout(layout),
    m_usage(usage),
    m_dirty(true),
    m_handle()
{
    auto& params = layout.parameters;

    m_bufferSize = 0;

    for (std::size_t i = 0; i < params.size(); ++i)
        m_bufferSize += params[i].first.size();

    m_buffer = new byte[m_bufferSize];

    std::size_t offset = 0;

    for (std::size_t i = 0; i < params.size(); ++i)
    {
        Parameter param(params[i].first, &m_buffer[offset], &m_dirty);

        m_parameters.push_back(std::move(param));
        m_parametersByName[params[i].second] = i;

        offset += params[i].first.size();
    }
}

inline MaterialParameterBuffer::~MaterialParameterBuffer()
{
    delete [] m_buffer;
}


inline rcptr<MaterialParameterBuffer> MaterialParameterBuffer::create(
    const char* name, const MaterialParameterBufferLayout& layout, Usage usage)
{
    return new MaterialParameterBuffer(name, layout, usage);
}


//////////////////////////////////////////////////////////////////////////
// Parameter management

inline int MaterialParameterBuffer::findParameter(const std::string& pname) const
{
    auto it = m_parametersByName.find(pname);
    return it != m_parametersByName.end() ? (int) it->second : -1;
}


inline const IMaterialParameter& MaterialParameterBuffer::parameter(const std::string& pname) const
{
    return parameter(findParameter(pname));
}

inline IMaterialParameter& MaterialParameterBuffer::parameter(const std::string& pname)
{
    return parameter(findParameter(pname));
}

inline IMaterialParameter& MaterialParameterBuffer::parameter(int index)
{
    static bool fakeDirty = false;
    static Parameter badParameter(MaterialParameterType::Int, nullptr, &fakeDirty);

    return index >= 0 ? m_parameters[(std::size_t) index] : badParameter;
}

inline const IMaterialParameter& MaterialParameterBuffer::parameter(int index) const
{
    return const_cast<MaterialParameterBuffer*>(this)->parameter(index);
}


inline const IMaterialParameter& MaterialParameterBuffer::operator[](const std::string& pname) const
{
    return parameter(pname);
}

inline IMaterialParameter& MaterialParameterBuffer::operator[](const std::string& pname)
{
    return parameter(pname);
}

inline const IMaterialParameter& MaterialParameterBuffer::operator[](int index) const
{
    return parameter(index);
}

inline IMaterialParameter& MaterialParameterBuffer::operator[](int index)
{
    return parameter(index);
}


inline const std::string& MaterialParameterBuffer::name() const
{
    return m_name;
}

inline const MaterialParameterBufferLayout& MaterialParameterBuffer::layout() const
{
    return m_layout;
}

inline MaterialParameterBuffer::Usage MaterialParameterBuffer::usage() const
{
    return m_usage;
}

inline void MaterialParameterBuffer::setUsage(Usage usage)
{
    m_usage = usage;
    m_dirty = true;
}

inline uint MaterialParameterBuffer::handle() const
{
    return m_handle;
}

} // ns gfx
} // ns mcr
