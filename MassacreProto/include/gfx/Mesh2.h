#pragma once

#include "VertexArray.h"
#include "FileSystem.h"

namespace mcr {
namespace gfx {

struct Mesh2
{
    rcptr<VertexArray> buffer;
    uint startIndex, numIndices;
    std::string hint;
};

void loadMeshes(std::vector<Mesh2>& out, VertexArray* va, IFile* file);

} // ns gfx
} // ns mcr
