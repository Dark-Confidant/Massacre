#pragma once

#include <mcr/FileSystem.h>
#include <mcr/gfx/experimental/Mesh.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

class IMeshExporter: public RefCounted
{
public:
    virtual ~IMeshExporter() {}
    virtual bool export_(const Mesh& mesh, const char* filename) const = 0;
};

rcptr<IMeshExporter> createSimpleMeshExporter();

} // ns experimental
} // ns gfx
} // ns mcr
