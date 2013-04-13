#pragma once

#include <FileSystem.h>
#include <gfx/experimental/Mesh.h>
#include <gfx/VertexFormat.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

class IMeshLoadTask: public RefCounted
{
public:
    struct MeshInfo
    {
        uint vertexDataSize, indexDataSize;
        VertexFormat vertexFormat;
    };

    virtual bool estimate(MeshInfo& infoOut) = 0;
    virtual bool load(const VertexFormat& bufferFormat, Mesh& mesh) = 0;
};

class IMeshLoader: public RefCounted
{
public:
    virtual ~IMeshLoader() {}
    virtual rcptr<IMeshLoadTask> createTask(IFile* file) const = 0;
};

rcptr<IMeshLoader> createSimpleMeshLoader();

} // ns experimental
} // ns gfx
} // ns mcr
