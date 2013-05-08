#pragma once

#include <mcr/gfx/IShaderPreprocessor.h>

namespace mcr {
namespace gfx {

class MaterialManager;

class ShaderPreprocessor: public IShaderPreprocessor
{
public:
    MCR_INTERN ShaderPreprocessor(MaterialManager* mgr);
    MCR_INTERN ~ShaderPreprocessor();

    MCR_INTERN bool preprocess(const char* source, std::vector<std::string>& sourceStringsOut);

private:
    struct DirectiveHandler;
    friend struct DirectiveHandler;

    MaterialManager* m_mm;
};

} // ns gfx
} // ns mcr
