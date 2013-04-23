namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// Creators

inline rcptr<GBuffer> GBuffer::create(Type type)
{
    return new GBuffer(type);
}

inline rcptr<GBuffer> GBuffer::create(Type type, uint size, Usage usage)
{
    auto buffer = new GBuffer(type);
    buffer->init(size, usage);
    return buffer;
}

//////////////////////////////////////////////////////////////////////////
// Accessors

inline GBuffer::Type GBuffer::type() const
{
    return m_type;
}

inline uint GBuffer::handle() const
{
    return m_handle;
}

} // ns gfx
} // ns mcr
