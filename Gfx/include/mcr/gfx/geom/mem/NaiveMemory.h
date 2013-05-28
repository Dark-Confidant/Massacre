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
    enum Target {Vertex, Index};
    enum Usage {Static, Dynamic};

    MCR_GFX_EXTERN NaiveMemory(Target target, Usage usage);

    MCR_GFX_EXTERN VideoHandle  allocate(std::size_t size);
    MCR_GFX_EXTERN void         free(VideoHandle handle);

    MCR_GFX_EXTERN void         read(VideoHandle src, std::size_t offset, std::size_t length, void* dst);
    MCR_GFX_EXTERN void         write(const void* src, std::size_t length, VideoHandle dst, std::size_t offset);

    bool isVirtual() const
    {
        return true;
    }

private:
    uint m_target, m_usage;
};

} // ns mem
} // ns geom
} // ns gfx
} // ns mcr
