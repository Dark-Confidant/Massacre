#pragma once

#include <string>
#include <mcr/FileSystem.h>
#include <mcr/math/Vector.h>
#include <mcr/gfx/ImageFormat.h>

namespace mcr {
namespace gfx {

class Image: public RefCounted
{
public:
    //! Create an empty image and allocate an appropriately sized buffer
    static rcptr<Image> create(const ivec2& size, const ImageFormat& fmt);

    //! Create an image and load its data form the specified file
    static rcptr<Image> createFromFile(IFile* file);


    //! Load image data of signature-derived format from the specified file
    MCR_EXTERN bool load(IFile* file);


    //! Image file name (if any)
    const std::string& filename() const { return m_filename; }

    //! Image pixel format
    const ImageFormat& format() const { return m_format; }

    //! Image dimensions in pixels
    const ivec2& size() const { return m_size; }

    //! Image raw data
    const byte* data() const { return m_data; }

    //! Image raw data; mutable, be careful
    byte* data() { return m_data; }

protected:
    Image();
    ~Image();

    std::string m_filename;

    ImageFormat m_format;
    ivec2 m_size;
    byte* m_data;
};

} // ns gfx
} // ns mcr

#include "Image.inl"
