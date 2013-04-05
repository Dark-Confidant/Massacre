#include "Universe.h"
#include "gfx/VertexPool.h"

using namespace mcr;
using namespace gfx;

VertexPool::VertexPool(const VertexFormat& fmt, uint vpc, uint ipc, uint chunks)
{
    auto vertexChunkStride = fmt.stride() * vpc;
    auto indexChunkStride  = (uint) sizeof(uint) * ipc;

    m_buffer = VertexArray::create(vertexChunkStride * chunks, indexChunkStride * chunks);
    m_buffer->setFormat(fmt);

    m_free.reserve(chunks);

    for (auto i = chunks; i--;)
    {
        Chunk chunk = {i * vertexChunkStride, i * indexChunkStride};
        m_free.emplace_back(chunk);
    }
}

VertexPool::~VertexPool() {}


VertexPool::Chunk VertexPool::borrowChunk()
{
    assert(m_free.size() > 0);

    Chunk chunk = m_free.back();
    m_free.pop_back();

    return chunk;
}

void VertexPool::returnChunk(const Chunk& chunk)
{
    m_free.push_back(chunk);
}
