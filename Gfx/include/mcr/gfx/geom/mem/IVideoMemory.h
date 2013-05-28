#pragma once

#include <mcr/Types.h>

namespace mcr  {
namespace gfx  {
namespace geom {
namespace mem  {

class IVideoMemory;

struct VideoHandle
{
    IVideoMemory* memory;
    union
    {
        void* ptr;
        struct { uint vbo, offset; };
    };
};

class IVideoMemory
{
public:
    virtual VideoHandle allocate(std::size_t size) = 0;
    virtual void        free(VideoHandle handle) = 0;

    virtual void        read(VideoHandle src, std::size_t offset, std::size_t length, void* dst) = 0;
    virtual void        write(const void* src, std::size_t length, VideoHandle dst, std::size_t offset) = 0;

    virtual bool        isVirtual() const = 0;
};

} // ns mem
} // ns geom
} // ns gfx
} // ns mcr
