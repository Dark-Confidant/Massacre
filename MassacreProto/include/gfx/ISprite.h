#pragma once

#include "math/Matrix.h"
#include "math/Rect.h"

namespace mcr {
namespace gfx {

class ISprite
{
public:
    virtual const mat4& transform() const = 0;
    virtual void setTransform(const mat4&) = 0;

    virtual const rect& baseRect() const = 0;
    virtual void setBaseRect(const rect&) = 0;

    virtual const rect& texRect() const = 0;
    virtual void setTexRect(const rect&) = 0;

    virtual const void* userAttribData(uint i) const = 0;
    virtual void setUserAttribData(uint i, const void* data) = 0;

    template <typename T>
    const T& userAttrib(uint i) const
    {
        return *static_cast<const T*>(userAttribData(i));
    }

    template <typename T>
    void setUserAttrib(uint i, const T& value)
    {
        setUserAttribData(i, &value);
    }

protected:
    ISprite() {}
    ~ISprite() {}
};

} // ns gfx
} // ns mcr
