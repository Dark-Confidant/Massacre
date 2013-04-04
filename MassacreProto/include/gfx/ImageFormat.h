#pragma once

#include "MassacreTypes.h"

namespace mcr {
namespace gfx {

class ImageFormat
{
public:
    ImageFormat();
    ~ImageFormat() {}

    static ImageFormat byEnum(uint openGLEnum);
    static ImageFormat byComponents(uint components, bool allowCompression = true);

    uint openGLEnum() const { return m_enum; }
    uint components() const { return m_components; }

protected:
    ImageFormat(uint openGLEnum, uint components);
    
    uint m_enum;
    uint m_components;
};

} // ns gfx
} // ns mcr