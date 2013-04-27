#pragma once

#include <mcr/math/Matrix.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

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


// TODO: type checking
class MaterialParameter
{
public:
    typedef void(*UploadFn)(const void* mem, void* hint);

    MaterialParameter(void* mem, UploadFn uploadFn = nullptr, void* hint = nullptr);
    ~MaterialParameter() {} // inherit not

    template    <typename T>
    const T&    get() const;

    template    <typename T>
    void        set(const T& value);

    template    <typename T>
    void        operator=(const T& rhs);

private:
    void *m_mem, *m_hint;
    UploadFn m_uploadFn;
};

} // ns experimental
} // ns gfx
} // ns mcr

#include "MaterialParameter.inl"
