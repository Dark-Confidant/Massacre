#pragma once

#include "RefCounted.h"

namespace mcr {
namespace gfx {

class GBuffer: public RefCounted
{
public:
    static rcptr<GBuffer> create(uint type)
    {
        return new GBuffer(type);
    }

    static rcptr<GBuffer> create(uint type, uint size, uint usage)
    {
        auto buffer = new GBuffer(type);
        buffer->init(size, usage);
        return buffer;
    }


    void init(uint size, uint usage);
    void init(const void* data, uint size, uint usage);

    uint type() const { return m_type; }
    uint handle() const { return m_handle; }

    uint capacity() const;

    void* map(uint access);
    void* map(uint offset, uint length, uint access);

    const void* map(uint access) const;
    const void* map(uint offset, uint length, uint access) const;

    void unmap() const;

protected:
    GBuffer(uint type);
    ~GBuffer();

    uint m_type, m_handle;
};

} // ns gfx
} // ns mcr
