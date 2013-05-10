#include "Universe.h"
#include <mcr/gfx/MeshLoader.h>

namespace mcr {
namespace gfx {

rcptr<VertexArray> MeshLoader::load(const MeshFileSet& files, std::vector<Mesh>& meshesOut)
{
    std::vector<rcptr<IMeshImportTask>> tasks;
    std::vector<Mesh> meshes;

    BOOST_FOREACH (auto& file, files.files)
        tasks.push_back(m_importer->createTask(m_fs->openFile(file.c_str())));

    IMeshImportTask::MeshInfo total;
    total.numVertices = total.numIndices = 0;

    BOOST_FOREACH (IMeshImportTask* task, tasks)
    {
        IMeshImportTask::MeshInfo info;

        if (!task->estimate(info))
            return nullptr;

        Mesh mesh =
        {
            nullptr,
            total.numVertices, info.numVertices,
            total.numIndices,  info.numIndices,
            PrimitiveType()
        };

        meshes.push_back(mesh);

        total.numVertices += info.numVertices;
        total.numIndices  += info.numIndices;

        if (total.vertexFormat.numAttribs() == 0)
            total.vertexFormat = info.vertexFormat;
    }

    auto buffer = VertexArray::create();

    buffer->setFormat(total.vertexFormat);
    buffer->vertices()->init(total.numVertices * total.vertexFormat.stride(), gfx::GBuffer::StaticDraw);
    buffer->indices()->init(total.numIndices * sizeof(uint), gfx::GBuffer::StaticDraw);

    for (uint i = 0; i < tasks.size(); ++i)
    {
        meshes[i].buffer = buffer;

        if (!tasks[i]->import(total.vertexFormat, meshes[i]))
            return nullptr;
    }

    meshesOut = meshes;

    return buffer;
}


} // ns gfx
} // ns mcr
