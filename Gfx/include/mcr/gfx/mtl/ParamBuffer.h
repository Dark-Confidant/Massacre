#pragma once

#include <string>
#include <vector>
#include <map>
#include <mcr/GfxExtern.h>
#include <mcr/gfx/mtl/ParamBufferBase.h>

namespace mcr {
namespace gfx {
namespace mtl {

class ParamBuffer: public ParamBufferBase
{
public:
    enum Usage: uint {Static, Dynamic, Stream};

    static rcptr<ParamBuffer> create(
        const char* name,
        const ParamLayout& layout,
        Usage usage = Stream);

    using ParamBufferBase::layout;

    const std::string&  name() const;

    Usage               usage() const;
    void                setUsage(Usage usage);

    uint                handle() const;

    MCR_GFX_EXTERN void init();
    MCR_GFX_EXTERN void sync();

protected:
    MCR_GFX_EXTERN ParamBuffer(const char* name, const ParamLayout& layout, Usage usage);
    MCR_GFX_EXTERN ~ParamBuffer();

private:
    void onInvalidateParam(int index, const void* data) { m_dirty = true; }

    const std::string   m_name;
    Usage               m_usage;
    bool                m_dirty;

    // implementation details
    uint                m_handle;
};

} // ns mtl
} // ns gfx
} // ns mcr

#include "ParamBuffer.inl"
