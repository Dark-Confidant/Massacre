namespace mcr {
namespace gfx {
namespace mtl {

inline bool operator==(const TextureFormat& lhs, const TextureFormat& rhs)
{
    return lhs.dataType == rhs.dataType
        && lhs.numComponents == rhs.numComponents
        && lhs.compressed == rhs.compressed;
}

inline bool operator!=(const TextureFormat& lhs, const TextureFormat& rhs)
{
    return !(lhs == rhs);
}


inline rcptr<Texture> Texture::create()
{
    return new Texture;
}

inline uint Texture::handle() const
{
    return m_handle;
}

inline const ivec2& Texture::size() const
{
    return m_size;
}

} // ns mtl
} // ns gfx
} // ns mcr
