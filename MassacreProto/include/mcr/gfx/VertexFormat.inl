namespace mcr {
namespace gfx {

inline uint VertexFormat::addAttrib(GType type, uint length, char semantic, uint offset)
{
    auto stride = offset + length * type.size();

    if (m_stride < stride)
        m_stride = stride;

    uint loc = (uint) m_attribs.size();

    Attrib attrib = {loc, type, length, offset, semantic};
    m_attribs.emplace_back(attrib);

    return loc;
}

inline uint VertexFormat::addAttrib(GType type, uint length, char semantic /*= '\0'*/)
{
    return addAttrib(type, length, semantic, m_stride);
}

inline uint VertexFormat::numAttribs() const
{
    return (uint) m_attribs.size();
}

inline const VertexFormat::Attrib& VertexFormat::attrib(uint i) const
{
    return m_attribs[i];
}

inline const VertexFormat::Attrib* VertexFormat::attribBySemantic(char semantic) const
{
    for (uint i = m_attribs.size(); i--;)
        if (m_attribs[i].semantic == semantic)
            return &m_attribs[i];

    return nullptr;
}

inline uint VertexFormat::stride() const
{
    return m_stride;
}

inline void VertexFormat::clear()
{
    m_attribs.clear();
    m_stride = 0;
}

} // ns gfx
} // ns mcr
