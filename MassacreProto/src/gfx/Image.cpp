#include "Universe.h"
#include "gfx/Image.h"

// TODO: move to pluggable loaders

#include <png.h>

// libjpeg-turbo redefines FAR
#ifdef FAR
#   undef FAR
#endif
#include <jpeglib.h>

namespace mcr {
namespace gfx {

bool Image::load(IFile* file)
{
    m_filename = file->filename();

    byte signature[4];

    if (file->size() < sizeof(signature))
        return false;

    file->read(signature);

    // DXT file
    if (strcmp((const char*) signature, "DXT ") == 0)
    {
        struct Header
        {
            uint fmt, w, h, size;
        }
        header;

        file->read(header);

        m_size = ivec2(header.w, header.h);
        m_format = ImageFormat::byGLEnum(header.fmt);

        m_data = new byte[header.size];

        file->read(m_data, header.size);

        return true;
    }

    // PNG file
    if (png_sig_cmp(signature, 0, sizeof(signature)) == 0)
    {
        auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        auto info = png_create_info_struct(png);

        struct PngCustomIO
        {
            static void read(png_structp png, png_bytep buffer, png_size_t size)
            {
                auto file = static_cast<IFile*>(png_get_io_ptr(png));
                file->read(buffer, size);
            }
        };

        png_set_read_fn(png, file, &PngCustomIO::read);
        png_set_sig_bytes(png, sizeof(signature));

        png_read_png(png, info, PNG_TRANSFORM_EXPAND, nullptr);

        m_size.set(png_get_image_width(png, info), png_get_image_height(png, info));
        m_format = ImageFormat::byComponents(png_get_channels(png, info), false);

        m_data = nullptr;

        auto colorType = png_get_color_type(png, info);

        if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_RGBA)
        {
            auto stride = png_get_rowbytes(png, info);
            auto rows   = png_get_rows(png, info);

            m_data = new byte[stride * m_size.y()];

            //*
            for (int y = m_size.y(), row = 0; y-- != 0; ++row)
                memcpy(&m_data[stride * y], rows[row], stride);
            /*/
            // HACK!
            for (int y = m_size.y(); y--;)
                memcpy(&m_data[stride * y], rows[y], stride);
            //*/
        }

        png_destroy_read_struct(&png, &info, nullptr);

        return m_data != nullptr;
    }

    // JPEG file
    if (signature[0] == 0xFF && signature[1] == 0xD8) // jpeg
    {
        file->seek(file->tell() - sizeof(signature));

        // custom error reporting
        struct my_error_mgr
        {
            jpeg_error_mgr pub;

            static void handle(j_common_ptr cinfo)
            {
                auto that = (my_error_mgr*) cinfo->err;
                that->pub.output_message(cinfo);

                longjmp(that->setjmp_buffer, 1);
            }

            jmp_buf setjmp_buffer;
        };

        // custom buffered reading
        struct my_source_mgr
        {
            jpeg_source_mgr pub;

            static void init_source(j_decompress_ptr cinfo) {}
            static void term_source(j_decompress_ptr cinfo) {}

            static boolean fill_input_buffer(j_decompress_ptr cinfo)
            {
                auto that = (my_source_mgr*) cinfo->src;

                cinfo->src->bytes_in_buffer = that->file->read(that->buffer);
                cinfo->src->next_input_byte = that->buffer;

                return cinfo->src->bytes_in_buffer != 0;
            }

            static void skip_input_data(j_decompress_ptr cinfo, long numBytes)
            {
                cinfo->src->bytes_in_buffer -= numBytes;
                cinfo->src->next_input_byte += numBytes;
            }

            IFile* file;
            byte buffer[4096];
        };

        jpeg_decompress_struct cinfo;

        my_error_mgr jerr;
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = &my_error_mgr::handle;

        if (setjmp(jerr.setjmp_buffer))
        {
            jpeg_destroy_decompress(&cinfo);
            return false;
        }
        
        jpeg_create_decompress(&cinfo);

        my_source_mgr src;
        /*auto& src = *(my_source_mgr*) cinfo.mem->alloc_small(
            (j_common_ptr) &cinfo, JPOOL_PERMANENT, sizeof(my_source_mgr));*/

        cinfo.src = &src.pub;
        src.pub.init_source = &my_source_mgr::init_source;
        src.pub.term_source = &my_source_mgr::term_source;
        src.pub.resync_to_restart = jpeg_resync_to_restart;
        src.pub.fill_input_buffer = &my_source_mgr::fill_input_buffer;
        src.pub.skip_input_data = &my_source_mgr::skip_input_data;
        src.pub.next_input_byte = nullptr;
        src.pub.bytes_in_buffer = 0;
        src.file = file;

        jpeg_read_header(&cinfo, true);
        jpeg_start_decompress(&cinfo);

        m_size.set((int) cinfo.output_width, (int) cinfo.output_height);
        m_format = ImageFormat::byComponents((uint) cinfo.output_components, false);

        m_data = nullptr;

        if (m_format.openGLEnum() == GL_RGB)
        {
            auto stride = cinfo.output_components * cinfo.output_width;
            auto length = stride * cinfo.output_height;

            auto row = cinfo.mem->alloc_sarray(
                (j_common_ptr) &cinfo, JPOOL_IMAGE, stride, 1);

            m_data = new byte[length];

            //*
            for (JDIMENSION offset = length; offset;)
            {
                offset -= stride;
                jpeg_read_scanlines(&cinfo, row, 1);
                memcpy(&m_data[offset], row[0], stride);
            }
            /*/
            // HACK!
            for (JDIMENSION offset = 0; offset < length; offset += stride)
            {
                jpeg_read_scanlines(&cinfo, row, 1);
                memcpy(&m_data[offset], row[0], stride);
            }
            //*/
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);

        return m_data != nullptr;
    }

    return false; // unrecognized format
}

} // ns gfx
} // ns mcr
