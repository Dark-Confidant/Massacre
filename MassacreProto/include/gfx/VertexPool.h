#pragma once

#include "VertexArray.h"

namespace mcr {
namespace gfx {

class VertexPool: public RefCounted
{
public:
    struct Chunk { uint voffset, ioffset; };

    static rcptr<VertexPool> create(
        const VertexFormat& fmt,
        uint vertPerChunk, uint indPerChunk,
        uint numChunks = 100 * 1024)
    {
        return new VertexPool(fmt, vertPerChunk, indPerChunk, numChunks);
    }

    Chunk borrowChunk();
    void returnChunk(const Chunk& chunk);

    const VertexArray* buffer() const { return m_buffer; }
    VertexArray* buffer() { return m_buffer; }

protected:
    VertexPool(const VertexFormat& fmt, uint vpc, uint ipc, uint chunks);
    ~VertexPool();

    rcptr<VertexArray> m_buffer;
    std::vector<Chunk> m_free;
};

} // ns gfx
} // ns mcr
