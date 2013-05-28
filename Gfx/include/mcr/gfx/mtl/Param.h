#pragma once

#include <mcr/math/Matrix.h>

namespace mcr {
namespace gfx {
namespace mtl {

struct ParamType
{
    enum Type: uint
    {
        Float, Double, Int,   UInt,
        Vec2,  DVec2,  IVec2, UVec2,
        Vec3,  DVec3,  IVec3, UVec3,
        Vec4,  DVec4,  IVec4, UVec4,
        Mat4,  DMat4
    }
    type;

    ParamType(Type atype);
    explicit ParamType(uint atype);

    operator Type() const;

    std::size_t size() const;
    std::size_t sizeAligned() const;
};


class IParam
{
public:
    ParamType                       type() const;

    template <typename T> bool      get(T& valueOut) const;
    template <typename T> bool      set(const T& value);

    template <typename T> IParam&   operator=(const T& rhs);

protected:
    IParam(ParamType type);
    ~IParam();

private:
    virtual const void* mem() const  = 0;
    virtual void*       mem()        = 0;
    virtual void        invalidate() = 0;

    ParamType m_type;
};

} // ns mtl
} // ns gfx
} // ns mcr

#include "Param.inl"
