#pragma once

#ifdef min
#   undef min
#endif
#ifdef max
#   undef max
#endif

#include <cmath>
#include <limits>

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

} // ns math
} // ns mcr
