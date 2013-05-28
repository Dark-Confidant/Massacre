namespace mcr {
namespace gfx {
namespace mtl {

inline rcptr<Image> Image::create(const ivec2& size, const ImageFormat& fmt)
{
    auto img = new Image;

    img->m_size = size;
    img->m_format = fmt;
    img->m_data = new byte[fmt.components() * size.x() * size.y()];

    return img;
}

inline rcptr<Image> Image::createFromFile(io::IFile* file)
{
    auto img = new Image;

    img->load(file);

    return img;
}

inline Image::Image(): m_data() {}

inline Image::~Image()
{
    if (m_data)
        delete [] m_data;
}

} // ns mtl
} // ns gfx
} // ns mcr
