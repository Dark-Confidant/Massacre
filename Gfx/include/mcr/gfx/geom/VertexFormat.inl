namespace mcr  {
namespace gfx  {
namespace geom {

inline uint AttribType::size() const
{
    static const uint s_sizes[] = {0, 1, 1, 2, 2, 4, 4, 4, 8};
    return s_sizes[type];
}

inline bool AttribType::valid() const
{
    return Unknown < type && type <= Double;
}


inline uint VertexFormat::addAttrib(AttribType type, uint length, char semantic, uint offset)
{
    auto stride = offset + length * type.size();

    if (m_stride < stride)
        m_stride = stride;

    uint loc = (uint) m_attribs.size();

    Attrib attrib = {loc, type, length, offset, semantic};
    m_attribs.emplace_back(attrib);

    return loc;
}

inline uint VertexFormat::addAttrib(AttribType type, uint length, char semantic /*= '\0'*/)
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

inline const VertexFormat::Attrib* VertexFormat::attribBySemantic(char semantic, uint nth) const
{
    for (uint i = m_attribs.size(); i--;)
        if (m_attribs[i].semantic == semantic)
            if (!nth--)
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

} // ns geom
} // ns gfx
} // ns mcr
