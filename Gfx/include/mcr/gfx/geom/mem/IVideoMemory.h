#pragma once

#include <mcr/RefCounted.h>

namespace mcr  {
namespace gfx  {
namespace geom {
namespace mem  {

class IVideoBuffer: public RefCounted
{
public:
    virtual void        read(std::size_t offset, std::size_t length, void* dst) const = 0;
    virtual void        write(std::size_t offset, std::size_t length, const void* src) = 0;

    virtual std::size_t size() const = 0;

    virtual uint        vbo() const = 0;
    virtual const void* offset() const = 0;
};

class IVideoMemory
{
public:
    virtual ~IVideoMemory() {}
    virtual rcptr<IVideoBuffer> allocate(std::size_t size) = 0;
};

} // ns mem
} // ns geom
} // ns gfx
} // ns mcr
