#pragma once

#ifdef min
#   undef min
#endif
#ifdef max
#   undef max
#endif

#include <cmath>
#include <limits>
#include <algorithm>

namespace mcr  {
namespace math {

static const float
    pi = 3.14159265358979323846f,
    deg2rad = pi / 180.f,
    rad2deg = 180.f / pi;

template <typename T>
inline bool equals(T a, T b, T t = std::numeric_limits<T>::epsilon())
{
    return a <= b + t && b <= a + t;
}

template <typename T>
inline const T& clamp(const T& a, const T& min, const T& max)
{
    return std::max(min, std::min(a, max));
}

inline byte numTrailingZeroes(uint a)
{
    static const byte s_mod37zeroes[] =
    {
        32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13,
        4, 7, 17, 0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9,
        5, 20, 8, 19, 18
    };

    return s_mod37zeroes[(a & (0u - a)) % 37u];
}

} // ns math
} // ns mcr
