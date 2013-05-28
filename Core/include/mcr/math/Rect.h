#pragma once

#include <mcr/math/Vector.h>

namespace mcr  {
namespace math {

template <typename T>
class Rect
{
public:
    Rect<T>():
        m_botLeft(), m_topRight() {}

    Rect<T>(T right, T top):
        m_botLeft(), m_topRight(right, top) {}

    Rect<T>(T left, T bottom, T right, T top):
        m_botLeft(left, bottom), m_topRight(right, top) {}

    Rect<T>(T left, T bottom, const Vector<T, 2>& topRight):
        m_botLeft(left, bottom), m_topRight(topRight) {}

    Rect<T>(const Vector<T, 2>& topRight):
        m_botLeft(), m_topRight(topRight) {}

    Rect<T>(const Vector<T, 2>& botLeft, T right, T top):
        m_botLeft(botLeft), m_topRight(right, top) {}

    Rect<T>(const Vector<T, 2>& botLeft, const Vector<T, 2>& topRight):
        m_botLeft(botLeft), m_topRight(topRight) {}

    Rect<T>(const Vector<T, 4>& vec):
        m_botLeft(vec.x(), vec.y()), m_topRight(vec.z(), vec.w()) {}

    template <typename R>
    Rect<T>(const Rect<R>& other):
        m_botLeft(other.botLeft()), m_topRight(other.topRight()) {}


    static Rect<T> bySize(T x, T y, T w, T h)
    {
        return Rect<T>(x, y, x + w, y + h);
    }

    static Rect<T> bySize(T x, T y, const Vector<T, 2>& size)
    {
        return Rect<T>(x, y, x + size.x(), y + size.y());
    }

    static Rect<T> bySize(const Vector<T, 2>& origin, const Vector<T, 2>& size)
    {
        return Rect<T>(origin, origin + size);
    }

    static Rect<T> bySize(const Vector<T, 2>& origin, T w, T h)
    {
        return Rect<T>(origin, origin + Vector<T, 2>(w, h));
    }


    T top   () const { return m_topRight.y(); }
    T left  () const { return m_botLeft.x();  }
    T right () const { return m_topRight.x(); }
    T bottom() const { return m_botLeft.y();  }

    const Vector<T, 2>& botLeft () const { return m_botLeft;  }
    const Vector<T, 2>& topRight() const { return m_topRight; }

    Vector<T, 2> topLeft () const { return Vector<T, 2>(left(), top());     }
    Vector<T, 2> botRight() const { return Vector<T, 2>(right(), bottom()); }

    T width () const { return m_topRight.x() - m_botLeft.x(); }
    T height() const { return m_topRight.y() - m_botLeft.y(); }

    Vector<T, 2> size() const { return m_topRight - m_botLeft; }


    const Vector<T, 2>& operator[](int i) const { return ((const Vector<T, 2>*) this)[i]; }
    Vector<T, 2>&       operator[](int i)       { return ((Vector<T, 2>*) this)[i]; }


    Rect<T>& operator+=(const Vector<T, 2>& rhs)
    {
        m_botLeft += rhs;
        m_topRight += rhs;

        return *this;
    }

    Rect<T>& operator-=(const Vector<T, 2>& rhs)
    {
        m_botLeft -= rhs;
        m_topRight -= rhs;

        return *this;
    }

    Rect<T>& operator*=(const Vector<T, 2>& rhs)
    {
        m_botLeft *= rhs;
        m_topRight *= rhs;

        return *this;
    }

    Rect<T>& operator/=(const Vector<T, 2>& rhs)
    {
        m_botLeft /= rhs;
        m_topRight /= rhs;

        return *this;
    }

protected:
    Vector<T, 2> m_botLeft, m_topRight;
};

template <typename T>
inline bool operator==(const Rect<T>& lhs, const Rect<T>& rhs)
{
    return lhs.topLeft() == rhs.topLeft() && lhs.botRight() == rhs.botRight();
}

template <typename T>
inline bool operator!=(const Rect<T>& lhs, const Rect<T>& rhs)
{
    return !(lhs == rhs);
}

template <typename T>
inline Rect<T> operator+(const Rect<T>& lhs, const Vector<T, 2>& rhs)
{
    return Rect<T>(lhs) += rhs;
}

template <typename T>
inline Rect<T> operator+(const Vector<T, 2>& lhs, const Rect<T>& rhs)
{
    return Rect<T>(rhs) += lhs;
}

template <typename T>
inline Rect<T> operator-(const Rect<T>& lhs, const Vector<T, 2>& rhs)
{
    return Rect<T>(lhs) -= rhs;
}

template <typename T>
inline Rect<T> operator-(const Vector<T, 2>& lhs, const Rect<T>& rhs)
{
    return Rect<T>(rhs) -= lhs;
}

template <typename T>
inline Rect<T> operator*(const Rect<T>& lhs, const Vector<T, 2>& rhs)
{
    return Rect<T>(lhs) *= rhs;
}

template <typename T>
inline Rect<T> operator*(const Vector<T, 2>& lhs, const Rect<T>& rhs)
{
    return Rect<T>(rhs) *= lhs;
}

template <typename T>
inline Rect<T> operator/(const Rect<T>& lhs, const Vector<T, 2>& rhs)
{
    return Rect<T>(lhs) /= rhs;
}

} // ns math

typedef math::Rect<float> rect;
typedef math::Rect<double> drect;

typedef math::Rect<int> irect;
typedef math::Rect<uint> urect;

} // ns mcr
