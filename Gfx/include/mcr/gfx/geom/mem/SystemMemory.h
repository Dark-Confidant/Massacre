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
    class Buffer: public IVideoBuffer
    {
    public:
        using RefCounted::operator new;
        using RefCounted::operator delete;

        Buffer(std::size_t size):
            m_size(size)
        {
            m_ptr = static_cast<byte*>(std::malloc(size));
        }

        ~Buffer()
        {
            std::free(m_ptr);
        }

        void read(std::size_t offset, std::size_t length, void* dst) const
        {
            std::memcpy(dst, m_ptr + offset, length);
        }

        void write(std::size_t offset, std::size_t length, const void* src)
        {
            std::memcpy(m_ptr + offset, src, length);
        }

        std::size_t size() const
        {
            return m_size;
        }

        uint vbo() const
        {
            return 0;
        }

        const void* offset() const
        {
            return m_ptr;
        }

    private:
        byte* m_ptr;
        std::size_t m_size;
    };

    rcptr<IVideoBuffer> allocate(std::size_t size)
    {
        return new Buffer(size);
    }
};

} // ns mem
} // ns geom
} // ns gfx
} // ns mcr
