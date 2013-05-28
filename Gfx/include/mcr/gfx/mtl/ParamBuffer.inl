namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Parameter

inline ParamBuffer::Param::Param(ParamType type, void* mem, bool* dirty):
IParam(type),
    m_mem(mem), m_dirty(dirty) {}

inline const void* ParamBuffer::Param::mem() const
{
    return m_mem;
}

inline void* ParamBuffer::Param::mem()
{
    return m_mem;
}

inline void ParamBuffer::Param::invalidate()
{
    *m_dirty = true;
}


//////////////////////////////////////////////////////////////////////////
// Buffer structor

inline rcptr<ParamBuffer> ParamBuffer::create(
    const char* name, const ParamLayout& layout, Usage usage)
{
    return new ParamBuffer(name, layout, usage);
}


//////////////////////////////////////////////////////////////////////////
// Parameter management

inline int ParamBuffer::findParam(const std::string& pname) const
{
    auto it = m_paramsByName.find(pname);
    return it != m_paramsByName.end() ? (int) it->second : -1;
}


inline const IParam& ParamBuffer::param(const std::string& pname) const
{
    return param(findParam(pname));
}

inline IParam& ParamBuffer::param(const std::string& pname)
{
    return param(findParam(pname));
}

inline IParam& ParamBuffer::param(int index)
{
    static bool fakeDirty = false;
    static Param badParam(ParamType::Int, nullptr, &fakeDirty);

    return index >= 0 ? m_params[(std::size_t) index] : badParam;
}

inline const IParam& ParamBuffer::param(int index) const
{
    return const_cast<ParamBuffer*>(this)->param(index);
}


inline const IParam& ParamBuffer::operator[](const std::string& pname) const
{
    return param(pname);
}

inline IParam& ParamBuffer::operator[](const std::string& pname)
{
    return param(pname);
}

inline const IParam& ParamBuffer::operator[](int index) const
{
    return param(index);
}

inline IParam& ParamBuffer::operator[](int index)
{
    return param(index);
}


inline const std::string& ParamBuffer::name() const
{
    return m_name;
}

inline const ParamLayout& ParamBuffer::layout() const
{
    return m_layout;
}

inline ParamBuffer::Usage ParamBuffer::usage() const
{
    return m_usage;
}

inline void ParamBuffer::setUsage(Usage usage)
{
    m_usage = usage;
    m_dirty = true;
}

inline uint ParamBuffer::handle() const
{
    return m_handle;
}

} // ns mtl
} // ns gfx
} // ns mcr
