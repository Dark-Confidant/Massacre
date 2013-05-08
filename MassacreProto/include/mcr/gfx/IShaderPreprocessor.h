#pragma once

#include <string>
#include <vector>

namespace mcr {
namespace gfx {

class IShaderPreprocessor
{
public:
    virtual ~IShaderPreprocessor() {}
    virtual bool preprocess(const char* source, std::vector<std::string>& sourceStringsOut) = 0;
};

} // ns gfx
} // ns mcr
