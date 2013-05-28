#pragma once

#include <mcr/GfxExtern.h>
#include <mcr/gfx/mtl/IShaderPreprocessor.h>

namespace mcr {
namespace gfx {
namespace mtl {

class Manager;

class ShaderPreprocessor: public IShaderPreprocessor
{
public:
    MCR_GFX_INTERN ShaderPreprocessor(Manager* mgr);
    MCR_GFX_INTERN ~ShaderPreprocessor();

    MCR_GFX_INTERN bool preprocess(const char* source, std::vector<std::string>& sourceStringsOut);

private:
    Manager* m_mm;
};

} // ns mtl
} // ns gfx
} // ns mcr
