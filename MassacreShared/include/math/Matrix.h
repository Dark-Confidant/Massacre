#pragma once

#include <utility>
#include "Vector.h"

namespace mcr  {
namespace math {

template <typename T>
class Matrix4x4
{
public:
    static const Matrix4x4<T>& I()
    {
        static struct Init
        {
            Init(): id(false)
            {
                for (size_t i = 0; i < 16; ++i)
                    id[i] = T(i / 4 == i % 4 ? 1 : 0);
            }
            Matrix4x4<T> id;
        }
        init;
        return init.id;
    }

    Matrix4x4<T>(bool setIdentity = true)
    {
        if (setIdentity)
            *this = I();
    }


    T& operator[](int i)
    { return m_elements[i]; }

    const T& operator[](int i) const
    { return m_elements[i]; }

    operator T*()
    { return m_elements; }

    operator const T*() const
    { return m_elements; }

    Matrix4x4<T>& operator=(const Matrix4x4<T>& rhs)
    {
        memcpy(m_elements, rhs.m_elements, sizeof(m_elements));
        return *this;
    }

    bool equals(const Matrix4x4<T>& rhs, T t = std::numeric_limits<T>::epsilon()) const
    {
        for (auto i = 0; i < 16; ++i)
            if (!math::equals(m_elements[i], rhs[i], t))
                return false;

        return true;
    }

    Matrix4x4<T>& transpose()
    {
        std::swap(m_elements[ 1], m_elements[ 4]);
        std::swap(m_elements[ 2], m_elements[ 8]);
        std::swap(m_elements[ 3], m_elements[12]);
        std::swap(m_elements[ 6], m_elements[ 9]);
        std::swap(m_elements[ 7], m_elements[13]);
        std::swap(m_elements[11], m_elements[14]);

        return *this;
    }

    bool inverse(Matrix4x4<T>& result)
    {
        Matrix4x4<T> inv(false);
        auto m = &m_elements[0];

        inv[ 0] = + m[ 5] * m[10] * m[15] - m[ 5] * m[11] * m[14] - m[ 9] * m[ 6] * m[15]
                  + m[ 9] * m[ 7] * m[14] + m[13] * m[ 6] * m[11] - m[13] * m[ 7] * m[10];
        inv[ 4] = - m[ 4] * m[10] * m[15] + m[ 4] * m[11] * m[14] + m[ 8] * m[ 6] * m[15]
                  - m[ 8] * m[ 7] * m[14] - m[12] * m[ 6] * m[11] + m[12] * m[ 7] * m[10];
        inv[ 8] = + m[ 4] * m[ 9] * m[15] - m[ 4] * m[11] * m[13] - m[ 8] * m[ 5] * m[15]
                  + m[ 8] * m[ 7] * m[13] + m[12] * m[ 5] * m[11] - m[12] * m[ 7] * m[ 9];
        inv[12] = - m[ 4] * m[ 9] * m[14] + m[ 4] * m[10] * m[13] + m[ 8] * m[ 5] * m[14]
                  - m[ 8] * m[ 6] * m[13] - m[12] * m[ 5] * m[10] + m[12] * m[ 6] * m[ 9];

        T det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        //if (math::equals(det, T(0)))
        if (det == T(0))
            return false;

        det = T(1) / det;

        inv[ 1] = - m[ 1] * m[10] * m[15] + m[ 1] * m[11] * m[14] + m[ 9] * m[ 2] * m[15]
                  - m[ 9] * m[ 3] * m[14] - m[13] * m[ 2] * m[11] + m[13] * m[ 3] * m[10];
        inv[ 5] = + m[ 0] * m[10] * m[15] - m[ 0] * m[11] * m[14] - m[ 8] * m[ 2] * m[15]
                  + m[ 8] * m[ 3] * m[14] + m[12] * m[ 2] * m[11] - m[12] * m[ 3] * m[10];
        inv[ 9] = - m[ 0] * m[ 9] * m[15] + m[ 0] * m[11] * m[13] + m[ 8] * m[ 1] * m[15]
                  - m[ 8] * m[ 3] * m[13] - m[12] * m[ 1] * m[11] + m[12] * m[ 3] * m[ 9];
        inv[13] = + m[ 0] * m[ 9] * m[14] - m[ 0] * m[10] * m[13] - m[ 8] * m[ 1] * m[14]
                  + m[ 8] * m[ 2] * m[13] + m[12] * m[ 1] * m[10] - m[12] * m[ 2] * m[ 9];
        inv[ 2] = + m[ 1] * m[ 6] * m[15] - m[ 1] * m[ 7] * m[14] - m[ 5] * m[ 2] * m[15]
                  + m[ 5] * m[ 3] * m[14] + m[13] * m[ 2] * m[ 7] - m[13] * m[ 3] * m[ 6];
        inv[ 6] = - m[ 0] * m[ 6] * m[15] + m[ 0] * m[ 7] * m[14] + m[ 4] * m[ 2] * m[15]
                  - m[ 4] * m[ 3] * m[14] - m[12] * m[ 2] * m[ 7] + m[12] * m[ 3] * m[ 6];
        inv[10] = + m[ 0] * m[ 5] * m[15] - m[ 0] * m[ 7] * m[13] - m[ 4] * m[ 1] * m[15]
                  + m[ 4] * m[ 3] * m[13] + m[12] * m[ 1] * m[ 7] - m[12] * m[ 3] * m[ 5];
        inv[14] = - m[ 0] * m[ 5] * m[14] + m[ 0] * m[ 6] * m[13] + m[ 4] * m[ 1] * m[14]
                  - m[ 4] * m[ 2] * m[13] - m[12] * m[ 1] * m[ 6] + m[12] * m[ 2] * m[ 5];
        inv[ 3] = - m[ 1] * m[ 6] * m[11] + m[ 1] * m[ 7] * m[10] + m[ 5] * m[ 2] * m[11]
                  - m[ 5] * m[ 3] * m[10] - m[ 9] * m[ 2] * m[ 7] + m[ 9] * m[ 3] * m[ 6];
        inv[ 7] = + m[ 0] * m[ 6] * m[11] - m[ 0] * m[ 7] * m[10] - m[ 4] * m[ 2] * m[11]
                  + m[ 4] * m[ 3] * m[10] + m[ 8] * m[ 2] * m[ 7] - m[ 8] * m[ 3] * m[ 6];
        inv[11] = - m[ 0] * m[ 5] * m[11] + m[ 0] * m[ 7] * m[ 9] + m[ 4] * m[ 1] * m[11]
                  - m[ 4] * m[ 3] * m[ 9] - m[ 8] * m[ 1] * m[ 7] + m[ 8] * m[ 3] * m[ 5];
        inv[15] = + m[ 0] * m[ 5] * m[10] - m[ 0] * m[ 6] * m[ 9] - m[ 4] * m[ 1] * m[10]
                  + m[ 4] * m[ 2] * m[ 9] + m[ 8] * m[ 1] * m[ 6] - m[ 8] * m[ 2] * m[ 5];
                      
        for (auto i = 0; i < 16; ++i)
            result[i] = inv[i] * det;

        return true;
    }

        
    const Vector<T, 3>& translation() const
    { return vecAt<3>(12); }

    void setTranslation(T x, T y, T z)
    { setTranslation(Vector<T, 3>(x, y, z)); }

    void setTranslation(const Vector<T, 3>& xyz)
    { vecAt<3>(12) = xyz; }

    void translate(T rx, T ry, T rz)
    { translate(Vector<T, 3>(rx, ry, rz)); }

    void translate(const Vector<T, 3>& rxyz)
    { vecAt<3>(12) += rxyz * *this; }

    void invertTranslation()
    { vecAt<3>(12) = -vecAt<3>(12); }


    void setRotation(T x, T y, T z)
    { setRotation(Vector<T, 3>(x, y, z)); }

    void setRotation(const Vector<T, 3>& angle)
    { setRotationRad(angle * deg2rad); }

    void setRotationRad(T x, T y, T z)
    { setRotationRad(Vector<T, 3>(x, y, z)); }

    void setRotationRad(const Vector<T, 3>& angleRad)
    {
        T   cr = cos(angleRad.x()),
            sr = sin(angleRad.x()),
            cp = cos(angleRad.y()),
            sp = sin(angleRad.y()),
            cy = cos(angleRad.z()),
            sy = sin(angleRad.z());

        m_elements[0] = cp * cy;
        m_elements[1] = cp * sy;
        m_elements[2] = -sp;

        T   srsp = sr * sp,
            crsp = cr * sp;

        m_elements[4] = srsp * cy - cr * sy;
        m_elements[5] = srsp * sy + cr * cy;
        m_elements[6] = sr * cp;

        m_elements[8]  = crsp * cy + sr * sy;
        m_elements[9]  = crsp * sy - sr * cy;
        m_elements[10] = cr * cp;
    }

    void invertRotation()
    {
        std::swap(m_elements[1], m_elements[4]);
        std::swap(m_elements[2], m_elements[8]);
        std::swap(m_elements[6], m_elements[9]);
    }


    void scaleUp(const vec3& scale)
    {
        vecAt<3>(0) *= scale.x();
        vecAt<3>(4) *= scale.y();
        vecAt<3>(8) *= scale.z();
    }

    void scaleDown(const vec3& scale)
    {
        vecAt<3>(0) /= scale.x();
        vecAt<3>(4) /= scale.y();
        vecAt<3>(8) /= scale.z();
    }


    template <int n>
    Vector<T, n>& vecAt(size_t offset)
    { return *reinterpret_cast<Vector<T, n>*>(&m_elements[offset]); }

    template <int n>
    const Vector<T, n>& vecAt(size_t offset) const
    { return *reinterpret_cast<const Vector<T, n>*>(&m_elements[offset]); }

protected:
    T m_elements[16];
};

template <typename T>
inline void buildTransform(
    Matrix4x4<T>& matrix,
    const Vector<T, 3>& position = Vector<T, 3>(),
    const Vector<T, 3>& rotation = Vector<T, 3>(),
    const Vector<T, 3>& scale    = Vector<T, 3>(T(1)))
{
    matrix.setRotation(rotation);
    matrix.scaleUp(scale);
    matrix.setTranslation(position);

    matrix[3] = matrix[7] = matrix[11] = T(0);
    matrix[15] = T(1);
}

template <typename T>
inline Matrix4x4<T> buildTransform(
    const Vector<T, 3>& position = Vector<T, 3>(),
    const Vector<T, 3>& rotation = Vector<T, 3>(),
    const Vector<T, 3>& scale    = Vector<T, 3>(T(1)))
{
    Matrix4x4<T> matrix(false);
    buildTransform(matrix, position, rotation, scale);

    return matrix;
}

template <typename T>
inline Matrix4x4<T> transpose(const Matrix4x4<T>& m)
{ return Matrix4x4<T>(m).transpose(); }

template <typename T>
inline bool operator==(const Matrix4x4<T>& lhs, const Matrix4x4<T>& rhs)
{
    return lhs.template vecAt<4>( 0) == rhs.template vecAt<4>( 0)
        && lhs.template vecAt<4>( 4) == rhs.template vecAt<4>( 4)
        && lhs.template vecAt<4>( 8) == rhs.template vecAt<4>( 8)
        && lhs.template vecAt<4>(12) == rhs.template vecAt<4>(12);
}

template <typename T>
inline Matrix4x4<T> operator*(const Matrix4x4<T>& lhs, const Matrix4x4<T>& rhs)
{
    Matrix4x4<T> r;

    r.template vecAt<4>( 0) = rhs.template vecAt<4>( 0) * lhs;
    r.template vecAt<4>( 4) = rhs.template vecAt<4>( 4) * lhs;
    r.template vecAt<4>( 8) = rhs.template vecAt<4>( 8) * lhs;
    r.template vecAt<4>(12) = rhs.template vecAt<4>(12) * lhs;

    return r;
}

template <typename T>
inline Vector<T, 2> operator*(const Vector<T, 2>& lhs, const Matrix4x4<T>& rhs)
{
    return lhs.x() * rhs.template vecAt<2>(0)
         + lhs.y() * rhs.template vecAt<2>(4);
}

template <typename T>
inline Vector<T, 3> operator*(const Vector<T, 3>& lhs, const Matrix4x4<T>& rhs)
{
    return lhs.x() * rhs.template vecAt<3>(0)
         + lhs.y() * rhs.template vecAt<3>(4)
         + lhs.z() * rhs.template vecAt<3>(8);
}

template <typename T>
inline Vector<T, 4> operator*(const Vector<T, 4>& lhs, const Matrix4x4<T>& rhs)
{
    return lhs.x() * rhs.template vecAt<4>(0)
         + lhs.y() * rhs.template vecAt<4>(4)
         + lhs.z() * rhs.template vecAt<4>(8)
         + lhs.w() * rhs.template vecAt<4>(12);
}

template <typename T, int n>
inline Vector<T, n> operator*(const Matrix4x4<T>& lhs, const Vector<T, n>& rhs)
{ return rhs * lhs; }

template <typename T>
inline Matrix4x4<T>& operator*=(Matrix4x4<T>& lhs, const Matrix4x4<T>& rhs)
{ return lhs = lhs * rhs; }

template <typename T, int n>
inline Vector<T, n>& operator*=(Vector<T, n>& lhs, const Matrix4x4<T>& rhs)
{ return lhs = rhs * lhs; }

template <typename T>
inline Matrix4x4<T>& operator+=(Matrix4x4<T>& lhs, const Vector<T, 3>& rhs)
{
    lhs.template vecAt<3>(12) += rhs;
    return lhs;
}

template <typename T>
inline Matrix4x4<T> operator+(const Matrix4x4<T>& lhs, const Vector<T, 3>& rhs)
{ return Matrix4x4<T>(lhs) += rhs; }

template <typename T>
inline Matrix4x4<T>& operator-=(Matrix4x4<T>& lhs, const Vector<T, 3>& rhs)
{
    lhs.template vecAt<3>(12) -= rhs;
    return lhs;
}

template <typename T>
inline Matrix4x4<T> operator-(const Matrix4x4<T>& lhs, const Vector<T, 3>& rhs)
{ return Matrix4x4<T>(lhs) -= rhs; }

} // ns math

typedef math::Matrix4x4<float> mat4;
typedef math::Matrix4x4<double> dmat4;

} // ns mcr
