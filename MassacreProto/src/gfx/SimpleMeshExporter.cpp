#include "Universe.h"
#include <mcr/gfx/IMeshExporter.h>

#include <fstream>
#include <SimpleMesh4.h>

namespace mcr {
namespace gfx {

namespace {
const uint g_primitiveTypeTr[] =
{
    GL_POINT,
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_TRIANGLE_FAN,
    GL_QUADS
};
} // ns

class SimpleMeshExporter: public IMeshExporter
{
public:
    using RefCounted::operator new;
    using RefCounted::operator delete;

    MCR_INTERN SimpleMeshExporter() {}
    MCR_INTERN ~SimpleMeshExporter() {}

    MCR_INTERN bool export_(const Mesh& mesh, const char* filename) const
    {
        auto& fmt = mesh.buffer->format();

        SimpleMesh::Header header;
        header.signature     = 0x4d53u;
        header.version       = SIMPLE_MESH_VERSION;
        header.flags         = 0;
        header.primitiveType = g_primitiveTypeTr[mesh.primitiveType];
        header.vertexSize    = fmt.stride();
        header.numAttributes = fmt.numAttribs();
        header.numVertices   = mesh.numVertices;
        header.numIndices    = mesh.numIndices;

        header.attributeDataOffset = sizeof(header);
        header.vertexDataOffset    = header.attributeDataOffset
                                   + header.numAttributes * sizeof(SimpleMesh::VertexAttribute);
        header.indexDataOffset     = header.vertexDataOffset
                                   + header.numVertices * header.vertexSize;

        std::ofstream file(filename, std::ios::binary);
        if (!file)
            return false;

        file.write((const char*) &header, sizeof(header));
        if (!file)
            return false;


        auto attribs = new SimpleMesh::VertexAttribute[header.numAttributes];

        for (uint i = 0; i < header.numAttributes; ++i)
        {
            attribs[i].type   = fmt.attrib(i).type - GType::SByte;
            attribs[i].length = fmt.attrib(i).length;
        }

        file.write((const char*) attribs, header.numAttributes * sizeof(SimpleMesh::VertexAttribute));

        delete [] attribs;

        if (!file)
            return false;


        file.write(
            (const char*) mesh.buffer->vertices()->map(
                header.vertexSize * mesh.startVertex,
                header.vertexSize * mesh.numVertices,
                GBuffer::Read),
            header.vertexSize * mesh.numVertices);

        file.write(
            (const char*) mesh.buffer->indices()->map(
                sizeof(uint) * mesh.startIndex,
                sizeof(uint) * mesh.numIndices,
                GBuffer::Read),
            sizeof(uint) * mesh.numIndices);

        mesh.buffer->vertices()->unmap();
        mesh.buffer->indices()->unmap();

        return !!file;
    }
};

rcptr<IMeshExporter> createSimpleMeshExporter()
{
    return new SimpleMeshExporter;
}

} // ns gfx
} // ns mcr
