#pragma once

#include <stdexcept>
#include <cassert>
#include "MassacreTypes.h"
#include "ExternalSingleton.h"

namespace mcr {

#define MCR_BULKY_SMALLOBJECTS

class SmallObjectAllocator: public ExternalSingleton<SmallObjectAllocator>
{
public:
    SmallObjectAllocator();
    ~SmallObjectAllocator();

    inline void* allocate(size_t size)
    {
        if      (size <= 8)   return pop(m_8);
        else if (size <= 16)  return pop(m_16);
        else if (size <= 32)  return pop(m_32);
        else if (size <= 64)  return pop(m_64);
        else if (size <= 128) return pop(m_128);
        else
            throw std::runtime_error("Small object too large");
    }

    inline void deallocate(void* mem)
    {
#ifndef MCR_BULKY_SMALLOBJECTS
        assert(this <= mem && mem < this + sizeof(*this));

        if      (mem < &m_16)  return push(m_8,   mem);
        else if (mem < &m_32)  return push(m_16,  mem);
        else if (mem < &m_64)  return push(m_32,  mem);
        else if (mem < &m_128) return push(m_64,  mem);
        else                   return push(m_128, mem);
#else
        BlockDesc* desc = (BlockDesc*) mem - 1;

        switch (desc->size)
        {
        case 8:   return push(m_8,   mem);
        case 16:  return push(m_16,  mem);
        case 32:  return push(m_32,  mem);
        case 64:  return push(m_64,  mem);
        case 128: return push(m_128, mem);
        default:
            throw std::runtime_error("Unknown size");
        }
#endif
    }

private:
    enum { MaxObjects = 1024 };

#ifdef MCR_BULKY_SMALLOBJECTS
    struct BlockDesc
    {
        byte size;
        //byte page;
    };
#endif

    template <size_t Size>
    struct Block
    {
#ifdef MCR_BULKY_SMALLOBJECTS
        BlockDesc desc;
#endif
        byte mem[Size];
    };

    template <size_t Size>
    struct List
    {
        size_t free;
        Block<Size> blocks[MaxObjects];
        void* stack[MaxObjects];
    };

    template <size_t Size>
    inline void* pop(List<Size>& list)
    {
        return list.stack[--list.free];
    }

    template <size_t Size>
    inline void push(List<Size>& list, void* mem)
    {
        list.stack[list.free++] = mem;
    }

    List<8>   m_8;
    List<16>  m_16;
    List<32>  m_32;
    List<64>  m_64;
    List<128> m_128;
};

class SmallObject
{
public:
    void* operator new(size_t size)
    {
        return SmallObjectAllocator::instance().allocate(size);
    }

    void operator delete(void* obj)
    {
        SmallObjectAllocator::instance().deallocate(obj);
    }
};

} // ns mcr
