#include "Universe.h"
#include "gfx/Mesh2.h"

#include <SimpleMesh4.h>

using namespace mcr;
using namespace gfx;
using namespace SimpleMesh;

#if 0
void gfx::loadMeshes(std::vector<Mesh2>& out, VertexArray* va, IFile* file)
{
    Header header;
    file->read(header);

    if (!header.checkSignature())
        return;

    if (header.version != SIMPLE_MESH_VERSION)
        return;


    // reading attributes

    VertexFormat fmt;

    auto attribs = new VertexAttribute[header.numAttributes];

    file->seek(header.attributeDataOffset);
    file->read(attribs, header.numAttributes);

    for (uint i = 0; i < header.numAttributes; ++i)
    {
        static const GLenum glTypes[] =
        {
            GL_BYTE,  GL_UNSIGNED_BYTE,
            GL_SHORT, GL_UNSIGNED_SHORT,
            GL_INT,   GL_UNSIGNED_INT,
            GL_FLOAT, GL_DOUBLE
        };

        fmt.addAttrib(glTypes[attribs[i].type], attribs[i].length);
    }

    delete [] attribs;

    m_buffer->setFormat(fmt);


    // reading vertices

    const uint vertexDataSize = header.vertexSize * header.numVertices;

    m_buffer->vertices()->init(vertexDataSize, GL_STATIC_DRAW);

    file->seek(header.vertexDataOffset);
    file->read(m_buffer->vertices()->map(GL_MAP_WRITE_BIT), vertexDataSize);

    m_buffer->vertices()->unmap();


    // reading indices

    const uint indexDataSize = sizeof(uint) * header.numIndices;

    m_buffer->indices()->init(indexDataSize, GL_STATIC_DRAW);

    file->seek(header.indexDataOffset);
    file->read(m_buffer->indices()->map(GL_MAP_WRITE_BIT), indexDataSize);

    m_buffer->indices()->unmap();


    // reading parts

    file->seek(header.partDataOffset);

    for (size_t i = 0; i < header.numParts; ++i)
    {
        Part part;
        file->read(part);

        Mesh2 mesh;

        mesh.startIndex = sizeof(uint) * part.start;
        mesh.numIndices = sizeof(uint) * part.size;

        file->readString0(mesh.hint);
    }
}
#endif
