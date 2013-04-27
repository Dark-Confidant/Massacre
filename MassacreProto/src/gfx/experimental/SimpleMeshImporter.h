#pragma once

#include <mcr/gfx/experimental/IMeshImporter.h>
#include <SimpleMesh4.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

class SimpleMeshImportTask: public IMeshImportTask
{
public:
    using RefCounted::operator new;
    using RefCounted::operator delete;

    SimpleMeshImportTask(IFile* file): m_file(file) {}

    bool estimate(MeshInfo& infoOut);
    bool import(const VertexFormat& bufferFormat, Mesh& mesh);

private:
    rcptr<IFile> m_file;
    MeshInfo m_info;
    SimpleMesh::Header m_header;
};

class SimpleMeshImporter: public IMeshImporter
{
public:
    using RefCounted::operator new;
    using RefCounted::operator delete;

    rcptr<IMeshImportTask> createTask(IFile* file) const
    {
        return new SimpleMeshImportTask(file);
    }
};

} // ns experimental
} // ns gfx
} // ns mcr
