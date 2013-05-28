#pragma once

#include <mcr/gfx/mtl/Param.h>

namespace mcr {
namespace gfx {
namespace mtl {

struct ParamLayout
{
    std::vector<std::pair<ParamType, std::string>> params;

    ParamLayout& add(ParamType type, const char* pname);

    ParamLayout& addFloat (const char* pname);
    ParamLayout& addVec2  (const char* pname);
    ParamLayout& addVec3  (const char* pname);
    ParamLayout& addVec4  (const char* pname);
    ParamLayout& addDouble(const char* pname);
    ParamLayout& addDVec2 (const char* pname);
    ParamLayout& addDVec3 (const char* pname);
    ParamLayout& addDVec4 (const char* pname);
    ParamLayout& addInt   (const char* pname);
    ParamLayout& addIVec2 (const char* pname);
    ParamLayout& addIVec3 (const char* pname);
    ParamLayout& addIVec4 (const char* pname);
    ParamLayout& addUInt  (const char* pname);
    ParamLayout& addUVec2 (const char* pname);
    ParamLayout& addUVec3 (const char* pname);
    ParamLayout& addUVec4 (const char* pname);
    ParamLayout& addMat4  (const char* pname);
    ParamLayout& addDMat4 (const char* pname);
};

} // ns mtl
} // ns gfx
} // ns mcr

#include "ParamLayout.inl"
