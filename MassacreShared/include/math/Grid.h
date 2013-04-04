#pragma once

#include <vector>
#include "Rect.h"
#include "Expr.h"

namespace mcr  {
namespace math {

class GridDimension
{
public:
    struct AxisInfo
    {
        AxisInfo(float aoffset, const Expr& aexpr = Expr(nullptr)):
            offset(aoffset), expr(aexpr) {}

        float offset;
        Expr expr;
    };

    void insertAxis(uint index, const Expr& expr)
    {
        if (m_axes.size() <= index)
            m_axes.resize(index + 1, AxisInfo(m_range[0]));

        m_axes[index] = AxisInfo(m_range[0], expr);

        if (expr.evaluate)
            m_axes[index].offset += expr.evaluate(m_range[1] - m_range[0]);
    }

    void removeAxis(uint axis)
    {
        m_axes[axis] = AxisInfo(m_range[0]);

        while (!m_axes.back().expr.evaluate)
            m_axes.pop_back(); 
    }

    uint numAxes() const
    {
        return (uint) m_axes.size();
    }

    float axis(uint axis) const
    {
        return m_axes[axis].offset;
    }

    vec2 interval(const uvec2& axes) const
    {
        return vec2(axis(axes.x()), axis(axes.y()));
    }

    const vec2& range() const
    {
        return m_range;
    }
    void setRange(const vec2& range)
    {
        m_range = range;

        float len = range[1] - range[0];

        for (uint i = 0; i < m_axes.size(); ++i)
            if (m_axes[i].expr.evaluate)
                m_axes[i].offset = range[0] + m_axes[i].expr.evaluate(len);
            else
                m_axes[i].offset = range[0];
    }

protected:
    std::vector<AxisInfo> m_axes;
    vec2 m_range;
};


template <int Dim> struct GridTraits {};

template <> struct GridTraits<1> { typedef vec2 Range; typedef uvec2 Interval; };
template <> struct GridTraits<2> { typedef rect Range; typedef urect Interval; };
//template <> struct GridTraits<3> { typedef bbox Range; typedef ubbox Interval; };


template <int Dim, typename Traits = GridTraits<Dim>>
class Grid
{
public:
    typedef typename Traits::Range Range;
    typedef typename Traits::Interval Interval;

    Range interval(const Interval& axes) const;

    Range range() const;
    void setRange(const Range& range);

    Range operator()(const Interval& axes) const { return interval(axes); }

    const GridDimension& operator[](int i) const { return m_dim[i]; }
    GridDimension&       operator[](int i)       { return m_dim[i]; }

protected:
    GridDimension m_dim[Dim];
};

template <>
inline Grid<2>::Range Grid<2>::interval(const Interval& axes) const
{
    auto iv0 = m_dim[0].interval(uvec2(axes[0][0], axes[1][0]));
    auto iv1 = m_dim[1].interval(uvec2(axes[0][1], axes[1][1]));

    return Range(iv0[0], iv1[0], iv0[1], iv1[1]);
}

template <>
inline Grid<2>::Range Grid<2>::range() const
{
    return Range(
        m_dim[0].range()[0], m_dim[1].range()[0],
        m_dim[0].range()[1], m_dim[1].range()[1]);
}

template <>
inline void Grid<2>::setRange(const Range& range)
{
    m_dim[0].setRange(vec2(range[0][0], range[1][0]));
    m_dim[1].setRange(vec2(range[0][1], range[1][1]));
}

} // ns math

typedef math::Grid<1> grid1;
typedef math::Grid<2> grid2;

} // ns mcr
