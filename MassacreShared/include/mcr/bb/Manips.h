#pragma once

#include <mcr/MassacreTypes.h>

namespace mcr {
namespace bb  {

//! DataBlock
struct DataBlock
{
    DataBlock(const void* aptr, uint asize):
        ptr(aptr), size(asize) {}

    const void* ptr;
    uint size;
};

template <typename Pod>
inline DataBlock data(const Pod& val)
{
    return DataBlock(&val, sizeof(Pod));
}

template <typename Pod>
inline DataBlock data(const Pod* ptr, size_t count)
{
    return DataBlock(ptr, sizeof(Pod) * count);
}

inline DataBlock data(const void* ptr, size_t size)
{
    return DataBlock(ptr, size);
}

template <typename RanIt>
inline DataBlock data(RanIt first, RanIt last)
{
    return data(&*first, last - first);
}

//! DataList
template <typename It>
struct DataList
{
    DataList(It first, It last):
        begin(first), end(last) {}

    It begin, end;
};

template <typename T>
inline DataList<const T*> list(const T* ptr, size_t count)
{
    return DataList<const T*>(ptr, ptr + count);
}

template <typename It>
inline DataList<It> list(It first, It last)
{
    return DataList<It>(first, last);
}

template <typename Container>
inline DataList<typename Container::const_iterator> list(const Container& cont)
{
    return list(cont.begin(), cont.end);
}

template <typename T, typename Alloc>
inline DataList<const T*> list(const std::vector<T, Alloc>& vec)
{
    return vec.size()
        ? list<const T*>(nullptr, nullptr)
        : list(&vec[0], &vec[0] + vec.size());
}

//! Counter
template <typename T>
struct counter
{
    counter(uint incInterval = 0): trigger(incInterval) {}

    typedef T type;
    const uint trigger;
};

//! Next - manually increment counter
enum Next { next };

//! End - terminate counter
enum End { end };

} // ns bb
} // ns mcr
