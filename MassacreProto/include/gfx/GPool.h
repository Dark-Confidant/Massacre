#pragma once

#include <vector>
#include "GBuffer.h"

namespace mcr {
namespace gfx {

class GPool: public RefCounted
{
public:
    static rcptr<GPool> create(
        GBuffer* buffer,
        uint blockSize, uint maxBlocks = 100000);

    uint alloc()
    {
        uint offset = m_offsets.back();
        m_offsets.pop_back();

        return offset;
    }

    void free(uint offset)
    {
        m_offsets.push_back(offset);
    }

    const GBuffer* buffer() const { return m_buffer; }
    GBuffer* buffer() { return m_buffer; }

    uint blockSize() const { return m_blockSize; }
    uint maxBlocks() const { return m_maxBlocks; }

protected:
    GPool(GBuffer* buffer, uint blockSize, uint maxBlocks);
    ~GPool();

    rcptr<GBuffer> m_buffer;
    std::vector<uint> m_offsets;
    const uint m_blockSize, m_maxBlocks;
};

} // ns gfx
} // ns mcr
