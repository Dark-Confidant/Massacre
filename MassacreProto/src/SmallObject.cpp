#include "Universe.h"
#include "SmallObject.h"

using namespace mcr;

SmallObjectAllocator* ExternalSingleton<SmallObjectAllocator>::m_instance;

SmallObjectAllocator::SmallObjectAllocator()
{
    memset(m_8.  blocks, 0, sizeof(m_8.  blocks));
    memset(m_16. blocks, 0, sizeof(m_16. blocks));
    memset(m_32. blocks, 0, sizeof(m_32. blocks));
    memset(m_64. blocks, 0, sizeof(m_64. blocks));
    memset(m_128.blocks, 0, sizeof(m_128.blocks));

    for (size_t i = 0; i < MaxObjects; ++i)
    {
        m_8.  stack[i] = m_8.  blocks[i].mem;
        m_16. stack[i] = m_16. blocks[i].mem;
        m_32. stack[i] = m_32. blocks[i].mem;
        m_64. stack[i] = m_64. blocks[i].mem;
        m_128.stack[i] = m_128.blocks[i].mem;

#ifdef MCR_BULKY_SMALLOBJECTS
        m_8.  blocks[i].desc.size = 8;
        m_16. blocks[i].desc.size = 16;
        m_32. blocks[i].desc.size = 32;
        m_64. blocks[i].desc.size = 64;
        m_128.blocks[i].desc.size = 128;
#endif
    }

    m_8.  free = MaxObjects;
    m_16. free = MaxObjects;
    m_32. free = MaxObjects;
    m_64. free = MaxObjects;
    m_128.free = MaxObjects;
}

SmallObjectAllocator::~SmallObjectAllocator()
{
    assert(m_8.  free == MaxObjects);
    assert(m_16. free == MaxObjects);
    assert(m_32. free == MaxObjects);
    assert(m_64. free == MaxObjects);
    assert(m_128.free == MaxObjects);
}
