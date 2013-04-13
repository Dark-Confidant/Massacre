#pragma once

#include <FileSystem.h>
#include <gfx/experimental/Mesh.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

class IMeshSaver: public RefCounted
{
public:
    virtual ~IMeshSaver() {}
    virtual bool save(const Mesh& mesh, const char* filename) const = 0;
};

rcptr<IMeshSaver> createSimpleMeshSaver();

} // ns experimental
} // ns gfx
} // ns mcr
