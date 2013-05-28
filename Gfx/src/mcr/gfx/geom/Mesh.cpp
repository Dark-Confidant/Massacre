#include "Universe.h"
#include <mcr/gfx/geom/Mesh.h>

#include <fstream>
#include <SimpleMesh4.h>

namespace mcr  {
namespace gfx  {
namespace geom {

bool Mesh::load(io::IFile* file, mem::IVideoMemory* vertMem, mem::IVideoMemory* idxMem, Mesh& meshOut)
{
    if (!file)
        return false;

    SimpleMesh::Header header;
    if (file->read(header) != sizeof(header))
        return false;

    if (!header.checkSignature() || header.version != SIMPLE_MESH_VERSION)
        return false;


    VertexFormat fmt;

    auto attribs = new SimpleMesh::VertexAttribute[header.numAttributes];

    file->seek(header.attributeDataOffset);
    file->read(attribs, header.numAttributes);

    for (uint i = 0; i < header.numAttributes; ++i)
        fmt.addAttrib(AttribType(AttribType::SByte + attribs[i].type), attribs[i].length); // sic!

    delete [] attribs;


    const std::size_t
        vertexDataSize = header.numVertices * header.vertexSize,
        indexDataSize  = header.numIndices * sizeof(uint);

    std::size_t read = 0;

    auto vertices = new byte[header.vertexSize * header.numVertices];
    auto indices  = new uint[header.numIndices];

    file->seek(header.vertexDataOffset);
    read += file->read(vertices, header.vertexSize * header.numVertices);

    file->seek(header.indexDataOffset);
    read += file->read(indices, header.numIndices);

    bool success = read == vertexDataSize + indexDataSize;
    if (success)
    {
        meshOut.vertices = vertMem->allocate(vertexDataSize);
        meshOut.indices  = idxMem->allocate(indexDataSize);

        vertMem->write(vertices, vertexDataSize, meshOut.vertices, 0);
        idxMem->write(indices, indexDataSize, meshOut.indices, 0);

        meshOut.vertexFormat  = fmt;
        meshOut.numVertices   = header.numVertices;
        meshOut.numIndices    = header.numIndices;
        meshOut.primitiveType = PrimitiveType::Triangles;
    }

    delete [] indices;
    delete [] vertices;

    return success;
}

bool Mesh::save(const Mesh& mesh, const char* filename)
{
    SimpleMesh::Header header;
    header.signature     = 0x4d53u;
    header.version       = SIMPLE_MESH_VERSION;
    header.flags         = 0;
    header.primitiveType = mesh.primitiveType;
    header.vertexSize    = mesh.vertexFormat.stride();
    header.numAttributes = mesh.vertexFormat.numAttribs();
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
        attribs[i].type   = mesh.vertexFormat.attrib(i).type - AttribType::SByte;
        attribs[i].length = mesh.vertexFormat.attrib(i).length;
    }

    file.write((const char*) attribs, header.numAttributes * sizeof(SimpleMesh::VertexAttribute));

    delete [] attribs;

    if (!file)
        return false;


    const std::size_t
        vertexDataSize = header.numVertices * header.vertexSize,
        indexDataSize  = header.numIndices * sizeof(uint);

    auto vertices = new char[vertexDataSize];
    auto indices  = new char[indexDataSize];

    mesh.vertices.memory->read(mesh.vertices, 0, vertexDataSize, vertices);
    mesh.indices.memory->read(mesh.indices, 0, indexDataSize, indices);

    file.write(vertices, vertexDataSize);
    file.write(indices, indexDataSize);

    delete [] indices;
    delete [] vertices;

    return !!file;
}

} // ns geom
} // ns gfx
} // ns mcr
