#pragma once

#include <vector>
#include <string>
#include <mcr/FileSystem.h>
#include <mcr/gfx/Mesh.h>

namespace mcr {
namespace gfx {

struct OldMeshes
{
    MCR_EXTERN static rcptr<VertexArray> load(
        IFile* file,
        std::vector<Mesh>& meshesOut,
        std::vector<std::string>* texNamesOut = nullptr);
};

} // ns gfx
} // ns mcr
