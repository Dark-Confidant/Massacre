#pragma once

#include <mcr/Types.h>

// TODO: own enums

namespace mcr {
namespace gfx {
namespace mtl {

class ImageFormat
{
public:
    ImageFormat();
    ~ImageFormat() {} // inherit not

    static ImageFormat byGLEnum(uint glEnum);
    static ImageFormat byComponents(uint components, bool allowCompression = true);

    uint toGLEnum() const { return m_enum; }
    uint components() const { return m_components; }

protected:
    ImageFormat(uint openGLEnum, uint components);
    
    uint m_enum;
    uint m_components;
};

} // ns mtl
} // ns gfx
} // ns mcr
