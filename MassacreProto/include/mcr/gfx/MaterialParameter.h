#pragma once

#include <mcr/math/Matrix.h>

namespace mcr {
namespace gfx {

struct MaterialParameterType
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

    MaterialParameterType(Type atype);
    explicit MaterialParameterType(uint atype);

    operator Type() const;

    std::size_t size() const;
};


class IMaterialParameter
{
public:
    MaterialParameterType       type() const;

    template <typename T> bool  get(T& valueOut) const;
    template <typename T> bool  set(const T& value);

    template <typename T> IMaterialParameter& operator=(const T& rhs);

protected:
    IMaterialParameter(MaterialParameterType type);
    ~IMaterialParameter();

private:
    virtual const void* mem() const  = 0;
    virtual void*       mem()        = 0;
    virtual void        invalidate() = 0;

    MaterialParameterType m_type;
};

} // ns gfx
} // ns mcr

#include "MaterialParameter.inl"
