#pragma once

#include <mcr/FileSystem.h>
#include <mcr/gfx/Mesh.h>

namespace mcr          {
namespace gfx          {

class IMeshExporter: public RefCounted
{
public:
    virtual bool export_(const Mesh& mesh, const char* filename) const = 0;
};

MCR_EXTERN rcptr<IMeshExporter> createSimpleMeshExporter();

} // ns gfx
} // ns mcr
