#pragma once

#include <cstring>
#include <cstdlib>
#include <mcr/gfx/geom/mem/IVideoMemory.h>

namespace mcr  {
namespace gfx  {
namespace geom {
namespace mem  {

class SystemMemory: public IVideoMemory
{
public:
    VideoHandle allocate(std::size_t size)
    {
        VideoHandle handle = {this, std::malloc(size)};
        return handle;
    }

    void free(VideoHandle handle)
    {
        std::free(handle.ptr);
    }

    void read(VideoHandle src, std::size_t offset, std::size_t length, void* dst)
    {
        std::memcpy(dst, (byte*) src.ptr + offset, length);
    }

    void write(const void* src, std::size_t length, VideoHandle dst, std::size_t offset)
    {
        std::memcpy((byte*) dst.ptr + offset, src, length);
    }

    bool isVirtual() const
    {
        return false;
    }
};

} // ns mem
} // ns geom
} // ns gfx
} // ns mcr
