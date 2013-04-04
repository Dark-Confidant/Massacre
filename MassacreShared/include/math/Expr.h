#pragma once

#include <functional>

namespace mcr  {
namespace math {

struct Expr
{
    Expr(const std::function<float(float)>& eval):
        evaluate(eval) {}

    Expr(float units):
        evaluate([units] (float) { return units; }) {}

    std::function<float(float)> evaluate;
};

inline Expr percents(float amount)
{
    return Expr([amount] (float limit) { return .01f * amount * limit; });
}

inline Expr operator+(const Expr& left, const Expr& right)
{
    return Expr([left, right] (float limit)
    {
        return left.evaluate(limit) + right.evaluate(limit);
    });
}

inline Expr operator-(const Expr& left, const Expr& right)
{
    return Expr([left, right] (float limit)
    {
        return left.evaluate(limit) - right.evaluate(limit);
    });
}

inline Expr operator*(const Expr& left, const Expr& right)
{
    return Expr([left, right] (float limit)
    {
        return left.evaluate(limit) * right.evaluate(limit);
    });
}

inline Expr operator/(const Expr& left, const Expr& right)
{
    return Expr([left, right] (float limit)
    {
        return left.evaluate(limit) / right.evaluate(limit);
    });
}

} // ns math
} // ns mcr
