#include "Universe.h"
#include "gfx/GPool.h"

using namespace mcr;
using namespace gfx;

GPool::GPool(GBuffer* buffer, uint blockSize, uint maxBlocks):
    m_buffer(buffer), m_blockSize(blockSize), m_maxBlocks(maxBlocks)
{
    const auto maxOffset = blockSize * maxBlocks;

    m_offsets.reserve(maxBlocks);

    for (uint offset = 0; offset < maxOffset; offset += blockSize)
        m_offsets.push_back(offset);
}

GPool::~GPool() {}
