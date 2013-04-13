#pragma once

#include <gfx/experimental/IMeshLoader.h>
#include <SimpleMesh4.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

class SimpleMeshLoadTask: public IMeshLoadTask
{
public:
    SimpleMeshLoadTask(IFile* file): m_file(file) {}

    bool estimate(MeshInfo& infoOut);
    bool load(const VertexFormat& bufferFormat, Mesh& mesh);

private:
    rcptr<IFile> m_file;
    MeshInfo m_info;
    SimpleMesh::Header m_header;
};

class SimpleMeshLoader: public IMeshLoader
{
public:
    rcptr<IMeshLoadTask> createTask(IFile* file) const
    {
        return new SimpleMeshLoadTask(file);
    }
};

} // ns experimental
} // ns gfx
} // ns mcr
