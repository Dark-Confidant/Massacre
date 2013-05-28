#pragma once

#include <string>
#include <vector>

namespace mcr {
namespace gfx {
namespace mtl {

class IShaderPreprocessor
{
public:
    virtual ~IShaderPreprocessor() {}
    virtual bool preprocess(const char* source, std::vector<std::string>& sourceStringsOut) = 0;
};

} // ns mtl
} // ns gfx
} // ns mcr
