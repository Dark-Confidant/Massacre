#pragma once

#include <mcr/Types.h>
#include <mcr/math/Math.h>

namespace mcr  {
namespace math {

template <typename T, int n>
class Vector;

template <typename T>
class Vector<T, 2>
{
public:
    Vector<T, 2>(): m_x(), m_y() {}
    explicit Vector<T, 2>(T t): m_x(t), m_y(t) {}
    Vector<T, 2>(T x, T y): m_x(x), m_y(y) {}
    Vector<T, 2>(const Vector<T, 2>& xy): m_x(xy.x()), m_y(xy.y()) {}

    T x() const { return m_x; }
    T y() const { return m_y; }

    void setX(T x) { m_x = x; }
    void setY(T y) { m_y = y; }

    void set(T x, T y)
    { m_x = x, m_y = y; }

    operator const T*() const
    { return reinterpret_cast<const T*>(&m_x); }

    operator T*()
    { return reinterpret_cast<T*>(&m_x); }

    template <typename R>
    operator Vector<R, 2>() const
    { return Vector<R, 2>(static_cast<R>(m_x), static_cast<R>(m_y)); }

    Vector<T, 2>& operator+=(const Vector<T, 2>& rhs)
    { return m_x += rhs.x(), m_y += rhs.y(), *this; }

    Vector<T, 2>& operator-=(const Vector<T, 2>& rhs)
    { return m_x -= rhs.x(), m_y -= rhs.y(), *this; }

    Vector<T, 2>& operator*=(const Vector<T, 2>& rhs)
    { return m_x *= rhs.x(), m_y *= rhs.y(), *this; }

    Vector<T, 2>& operator/=(const Vector<T, 2>& rhs)
    { return m_x /= rhs.x(), m_y /= rhs.y(), *this; }

    Vector<T, 2>& operator*=(T rhs)
    { return m_x *= rhs, m_y *= rhs, *this; }

    Vector<T, 2>& operator/=(T rhs)
    { return m_x /= rhs, m_y /= rhs, *this; }

protected:
    T m_x, m_y;
};

template <typename T>
class Vector<T, 3>
{
public:
    Vector<T, 3>(): m_x(), m_y(), m_z() {}
    explicit Vector<T, 3>(T t): m_x(t), m_y(t), m_z(t) {}
    Vector<T, 3>(T x, T y, T z): m_x(x), m_y(y), m_z(z) {}
    Vector<T, 3>(const Vector<T, 2>& xy, T z): m_x(xy.x()), m_y(xy.y()), m_z(z) {}
    Vector<T, 3>(T x, const Vector<T, 2>& yz): m_x(x), m_y(yz.x()), m_z(yz.y()) {}
    Vector<T, 3>(const Vector<T, 3>& xyz): m_x(xyz.x()), m_y(xyz.y()), m_z(xyz.z()) {}

    T x() const { return m_x; }
    T y() const { return m_y; }
    T z() const { return m_z; }

    void setX(T x) { m_x = x; }
    void setY(T y) { m_y = y; }
    void setZ(T z) { m_z = z; }

    void set(T x, T y, T z)
    { m_x = x, m_y = y, m_z = z; }

    operator const T*() const
    { return reinterpret_cast<const T*>(&m_x); }

    operator T*()
    { return reinterpret_cast<T*>(&m_x); }

    template <typename R>
    operator Vector<R, 3>() const
    { return Vector<R, 3>(static_cast<R>(m_x), static_cast<R>(m_y), static_cast<R>(m_z)); }

    Vector<T, 3>& operator+=(const Vector<T, 3>& rhs)
    { return m_x += rhs.x(), m_y += rhs.y(), m_z += rhs.z(), *this; }

    Vector<T, 3>& operator-=(const Vector<T, 3>& rhs)
    { return m_x -= rhs.x(), m_y -= rhs.y(), m_z -= rhs.z(), *this; }

    Vector<T, 3>& operator*=(const Vector<T, 3>& rhs)
    { return m_x *= rhs.x(), m_y *= rhs.y(), m_z *= rhs.z(), *this; }

    Vector<T, 3>& operator/=(const Vector<T, 3>& rhs)
    { return m_x /= rhs.x(), m_y /= rhs.y(), m_z /= rhs.z(), *this; }

    Vector<T, 3>& operator*=(T rhs)
    { return m_x *= rhs, m_y *= rhs, m_z *= rhs, *this; }

    Vector<T, 3>& operator/=(T rhs)
    { return m_x /= rhs, m_y /= rhs, m_z /= rhs, *this; }

protected:
    T m_x, m_y, m_z;
};

template <typename T>
class Vector<T, 4>
{
public:
    Vector<T, 4>(): m_x(), m_y(), m_z(), m_w() {}
    explicit Vector<T, 4>(T t): m_x(t), m_y(t), m_z(t), m_w(t) {}
    Vector<T, 4>(T x, T y, T z, T w): m_x(x), m_y(y), m_z(z), m_w(w) {}
    Vector<T, 4>(const Vector<T, 2>& xy, T z, T w): m_x(xy.x()), m_y(xy.y()), m_z(z), m_w(w) {}
    Vector<T, 4>(T x, T y, const Vector<T, 2>& zw): m_x(x), m_y(y), m_z(zw.x()), m_w(zw.y()) {}
    Vector<T, 4>(const Vector<T, 2>& xy, const Vector<T, 2>& zw): m_x(xy.x()), m_y(xy.y()), m_z(zw.x()), m_w(zw.y()) {}
    Vector<T, 4>(const Vector<T, 3>& xyz, T w): m_x(xyz.x()), m_y(xyz.y()), m_z(xyz.z()), m_w(w) {}
    Vector<T, 4>(T x, const Vector<T, 3>& yzw): m_x(x), m_y(yzw.x()), m_z(yzw.y()), m_w(yzw.z()) {}
    Vector<T, 4>(const Vector<T, 4>& xyzw): m_x(xyzw.x()), m_y(xyzw.y()), m_z(xyzw.z()), m_w(xyzw.w()) {}

    T x() const { return m_x; }
    T y() const { return m_y; }
    T z() const { return m_z; }
    T w() const { return m_w; }

    void setX(T x) { m_x = x; }
    void setY(T y) { m_y = y; }
    void setZ(T z) { m_z = z; }
    void setW(T w) { m_w = w; }

    void set(T x, T y, T z, T w)
    { m_x = x, m_y = y, m_z = z, m_w = w; }

    operator const T*() const
    { return reinterpret_cast<const T*>(&m_x); }

    operator T*()
    { return reinterpret_cast<T*>(&m_x); }

    template <typename R>
    operator Vector<R, 4>() const
    { return Vector<R, 4>(static_cast<R>(m_x), static_cast<R>(m_y), static_cast<R>(m_z), static_cast<R>(m_w)); }

    Vector<T, 4>& operator+=(const Vector<T, 4>& rhs)
    { return m_x += rhs.x(), m_y += rhs.y(), m_z += rhs.z(), m_w += rhs.w(), *this; }

    Vector<T, 4>& operator-=(const Vector<T, 4>& rhs)
    { return m_x -= rhs.x(), m_y -= rhs.y(), m_z -= rhs.z(), m_w == rhs.w(), *this; }

    Vector<T, 4>& operator*=(const Vector<T, 4>& rhs)
    { return m_x *= rhs.x(), m_y *= rhs.y(), m_z *= rhs.z(), m_w *= rhs.w(), *this; }

    Vector<T, 4>& operator/=(const Vector<T, 4>& rhs)
    { return m_x /= rhs.x(), m_y /= rhs.y(), m_z /= rhs.z(), m_w /= rhs.w(), *this; }

    Vector<T, 4>& operator*=(T rhs)
    { return m_x *= rhs, m_y *= rhs, m_z *= rhs, m_w *= rhs, *this; }

    Vector<T, 4>& operator/=(T rhs)
    { return m_x /= rhs, m_y /= rhs, m_z /= rhs, m_w /= rhs, *this; }

protected:
    T m_x, m_y, m_z, m_w;
};


template <typename T>
inline bool equals(const Vector<T, 2>& a, const Vector<T, 2>& b)
{ return equals(a.x(), b.x()) && equals(a.y(), b.y()); }

template <typename T>
inline bool equals(const Vector<T, 3>& a, const Vector<T, 3>& b)
{ return equals(a.x(), b.x()) && equals(a.y(), b.y()) && equals(a.z(), b.z()); }

template <typename T>
inline bool equals(const Vector<T, 4>& a, const Vector<T, 4>& b)
{ return equals(a.x(), b.x()) && equals(a.y(), b.y()) && equals(a.z(), b.z()) && equals(a.w(), b.w()); }

template <typename T>
inline Vector<T, 3> cross(const Vector<T, 3>& a, const Vector<T, 3>& b)
{
    return Vector<T, 3>(
        a.y() * b.z() - a.z() * b.y(),
        a.z() * b.x() - a.x() * b.z(),
        a.x() * b.y() - a.y() * b.x());
}

template <typename T>
inline T sum(const Vector<T, 2>& a)
{ return a.x() + a.y(); }

template <typename T>
inline T sum(const Vector<T, 3>& a)
{ return a.x() + a.y() + a.z(); }

template <typename T>
inline T sum(const Vector<T, 4>& a)
{ return a.x() + a.y() + a.z() + a.w(); }

template <typename T>
inline T mulc(const Vector<T, 2>& a)
{ return a.x() * a.y(); }

template <typename T>
inline T mulc(const Vector<T, 3>& a)
{ return a.x() * a.y() * a.z(); }

template <typename T>
inline T mulc(const Vector<T, 4>& a)
{ return a.x() * a.y() * a.z() * a.w(); }

template <typename T>
inline T area(const Vector<T, 2>& a)
{ return mulc(a); }

template <typename T>
inline T volume(const Vector<T, 3>& a)
{ return mulc(a); }

template <typename T>
inline T maxc(const Vector<T, 2>& a)
{ return std::max(a.x(), a.y()); }

template <typename T>
inline T maxc(const Vector<T, 3>& a)
{ return std::max(a.x(), std::max(a.y(), a.z())); }

template <typename T>
inline T maxc(const Vector<T, 4>& a)
{ return std::max(std::max(a.x(), a.y()), std::max(a.z(), a.w())); }

template <typename T>
inline T minc(const Vector<T, 2>& a)
{ return std::min(a.x(), a.y()); }

template <typename T>
inline T minc(const Vector<T, 3>& a)
{ return std::min(a.x(), std::min(a.y(), a.z())); }

template <typename T>
inline T minc(const Vector<T, 4>& a)
{ return std::min(std::min(a.x(), a.y()), std::min(a.z(), a.w())); }

template <typename T, int n>
inline T dot(const Vector<T, n>& a, const Vector<T, n>& b)
{ return sum(a * b); }

template <typename T, int n>
inline T lengthSq(const Vector<T, n>& a)
{ return dot(a, a); }

template <typename T, int n>
inline T length(const Vector<T, n>& a)
{ return sqrt(lengthSq(a)); }

template <typename T, int n>
inline Vector<T, n> normalize(const Vector<T, n>& a)
{
    auto len = length(a);

    if (equals(len, T(0)))
        return a;

    return a / len;
}


template <typename T, int n>
inline bool operator==(const Vector<T, n>& lhs, const Vector<T, n>& rhs)
{ return equals(lhs, rhs); }

template <typename T, int n>
inline bool operator!=(const Vector<T, n>& lhs, const Vector<T, n>& rhs)
{ return !(lhs == rhs); }

template <typename T, int n>
inline Vector<T, n> operator-(const Vector<T, n>& rhs)
{ return Vector<T, n>() - rhs; }

template <typename T, int n>
inline Vector<T, n> operator+(const Vector<T, n>& lhs, const Vector<T, n>& rhs)
{ return Vector<T, n>(lhs) += rhs; }

template <typename T, int n>
inline Vector<T, n> operator-(const Vector<T, n>& lhs, const Vector<T, n>& rhs)
{ return Vector<T, n>(lhs) -= rhs; }

template <typename T, int n>
inline Vector<T, n> operator*(const Vector<T, n>& lhs, const Vector<T, n>& rhs)
{ return Vector<T, n>(lhs) *= rhs; }

template <typename T, int n>
inline Vector<T, n> operator/(const Vector<T, n>& lhs, const Vector<T, n>& rhs)
{ return Vector<T, n>(lhs) /= rhs; }

template <typename T, int n>
inline Vector<T, n> operator*(const Vector<T, n>& lhs, T rhs)
{ return Vector<T, n>(lhs) *= rhs; }

template <typename T, int n>
inline Vector<T, n> operator/(const Vector<T, n>& lhs, T rhs)
{ return Vector<T, n>(lhs) /= rhs; }

template <typename T, int n>
inline Vector<T, n> operator*(T lhs, const Vector<T, n>& rhs)
{ return rhs * lhs; }

} // ns math

typedef math::Vector<float, 2> vec2;
typedef math::Vector<float, 3> vec3;
typedef math::Vector<float, 4> vec4;

typedef math::Vector<double, 2> dvec2;
typedef math::Vector<double, 3> dvec3;
typedef math::Vector<double, 4> dvec4;

typedef math::Vector<int, 2> ivec2;
typedef math::Vector<int, 3> ivec3;
typedef math::Vector<int, 4> ivec4;

typedef math::Vector<uint, 2> uvec2;
typedef math::Vector<uint, 3> uvec3;
typedef math::Vector<uint, 4> uvec4;

typedef math::Vector<byte, 4> u8vec4;
typedef math::Vector<short, 4> u16vec4;

} // ns mcr
