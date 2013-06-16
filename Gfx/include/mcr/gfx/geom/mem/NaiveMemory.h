#pragma once

#include <mcr/GfxExtern.h>
#include <mcr/gfx/geom/mem/IVideoMemory.h>

namespace mcr  {
namespace gfx  {
namespace geom {
namespace mem  {

class NaiveMemory: public IVideoMemory
{
public:
    class Buffer: public IVideoBuffer
    {
    public:
        using RefCounted::operator new;
        using RefCounted::operator delete;

        MCR_GFX_EXTERN Buffer(const NaiveMemory* memory, std::size_t size);
        MCR_GFX_EXTERN ~Buffer();

        MCR_GFX_EXTERN void read(std::size_t offset, std::size_t length, void* dst) const;
        MCR_GFX_EXTERN void write(std::size_t offset, std::size_t length, const void* src);

        std::size_t size() const { return m_size; }

        uint        vbo() const { return m_vbo; }
        const void* offset() const { return 0; }

    private:
        const NaiveMemory* m_memory;
        std::size_t m_size;
        uint m_vbo;
    };

    enum Target {Vertex, Index};
    enum Usage {Static, Dynamic};

    MCR_GFX_EXTERN NaiveMemory(Target target, Usage usage);

    MCR_GFX_EXTERN rcptr<IVideoBuffer> allocate(std::size_t size);

private:
    friend class Buffer;

    uint m_target, m_usage;
};

} // ns mem
} // ns geom
} // ns gfx
} // ns mcr
