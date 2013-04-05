#pragma once

#include "Image.h"

namespace mcr {
namespace gfx {

class Texture: public RefCounted
{
public:
    //! Create an empty texture
    static rcptr<Texture> create();

    /*! Create a texture from file contents
        \param file File handle provided by \c FileSystem
        \param upload Whether to upload image data to GPU
        \param clearLocal Whether to clear local image data after uploading
    */
    static rcptr<Texture> createFromFile(IFile* file, rcptr<Image>* imgOut = nullptr);


    //! Load image from PNG or JPEG file
    void load(IFile* file);


    //! Upload image data to GPU
    void upload(Image* img);

    //! Whether image data resides on GPU
    bool isUploaded() { return m_uploaded; }

    //! Remove image data from GPU
    void clear();


    //! Download image data from GPU to local storage
    rcptr<Image> download();


    //! Image file name (if any)
    const std::string& filename() const { return m_filename; }

    //! OpenGL object handle
    uint handle() const { return m_handle; }

    //! Image pixel format
    const ImageFormat& format() const { return m_format; }

    //! Image dimensions
    const ivec2& size() const { return m_size; }

protected:
    Texture();
    ~Texture();

    std::string m_filename;

    uint m_handle;

    ImageFormat m_format;
    ivec2 m_size;
    bool m_uploaded;
};

} // ns gfx
} // ns mcr
