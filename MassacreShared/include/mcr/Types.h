#pragma once

#include <mcr/BuildConfig.h>

namespace mcr {

typedef signed char sbyte;
typedef long long int64;

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long uint64;

template <typename T, size_t n>
inline size_t arrsize(T(&)[n]) { return n; }

} // ns mcr

#ifndef MCR_STRICT_TYPES
using mcr::sbyte;
using mcr::int64;
using mcr::byte;
using mcr::ushort;
using mcr::uint;
using mcr::uint64;
#endif
