namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Buffer structor

inline rcptr<ParamBuffer> ParamBuffer::create(
    const char* name, const ParamLayout& layout, Usage usage)
{
    return new ParamBuffer(name, layout, usage);
}


//////////////////////////////////////////////////////////////////////////
// Accessors & mutators

inline const std::string& ParamBuffer::name() const
{
    return m_name;
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
