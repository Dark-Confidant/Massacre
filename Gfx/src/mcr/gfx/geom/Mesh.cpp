#include "Universe.h"
#include <mcr/gfx/geom/Mesh.h>

#include <fstream>
#include <SimpleMesh4.h>

namespace mcr  {
namespace gfx  {
namespace geom {

bool Mesh::load(io::IFileReader* stream, mem::IVideoMemory* vertMem, mem::IVideoMemory* idxMem, Mesh& meshOut)
{
    if (!stream)
        return false;

    auto startPos = stream->tell();

    SimpleMesh::Header header;
    if (stream->read(header) != sizeof(header))
        return false;

    if (!header.checkSignature() || header.version != SIMPLE_MESH_VERSION)
        return false;


    const std::size_t
        attribDataSize = header.numAttributes * sizeof(SimpleMesh::VertexAttribute),
        vertexDataSize = header.numVertices * header.vertexSize,
        indexDataSize  = header.numIndices * sizeof(uint);

    std::size_t read = sizeof(header);


    VertexFormat fmt;

    auto attribs = new SimpleMesh::VertexAttribute[header.numAttributes];

    stream->seek(startPos + header.attributeDataOffset);
    read += stream->read(attribs, header.numAttributes);

    for (uint i = 0; i < header.numAttributes; ++i)
        fmt.addAttrib(AttribType(AttribType::SByte + attribs[i].type), attribs[i].length); // sic!

    delete [] attribs;


    auto vertices = new byte[vertexDataSize];
    auto indices  = new uint[header.numIndices];

    stream->seek(startPos +header.vertexDataOffset);
    read += stream->read(vertices, vertexDataSize);

    stream->seek(startPos +header.indexDataOffset);
    read += stream->read(indices, header.numIndices);

    bool success = read == sizeof(header) + attribDataSize + vertexDataSize + indexDataSize;
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

bool Mesh::save(io::IWriter* stream, const Mesh& mesh)
{
    if (!stream)
        return false;

    SimpleMesh::Header header;
    header.signature     = 0x4d53u;
    header.version       = SIMPLE_MESH_VERSION;
    header.flags         = 0;
    header.primitiveType = mesh.primitiveType;
    header.vertexSize    = mesh.vertexFormat.stride();
    header.numAttributes = mesh.vertexFormat.numAttribs();
    header.numVertices   = mesh.numVertices;
    header.numIndices    = mesh.numIndices;

    const std::size_t
        attribDataSize = header.numAttributes * sizeof(SimpleMesh::VertexAttribute),
        vertexDataSize = header.numVertices * header.vertexSize,
        indexDataSize  = header.numIndices * sizeof(uint);

    header.attributeDataOffset = sizeof(header);
    header.vertexDataOffset    = header.attributeDataOffset + attribDataSize;
    header.indexDataOffset     = header.vertexDataOffset + vertexDataSize;

    std::size_t written = stream->write(header);


    for (uint i = 0; i < header.numAttributes; ++i)
    {
        SimpleMesh::VertexAttribute attrib;

        attrib.type   = mesh.vertexFormat.attrib(i).type - AttribType::SByte;
        attrib.length = mesh.vertexFormat.attrib(i).length;

        written += stream->write(attrib);
    }


    auto vertices = new byte[vertexDataSize];
    auto indices  = new byte[indexDataSize];

    mesh.vertices.memory->read(mesh.vertices, 0, vertexDataSize, vertices);
    mesh.indices.memory->read(mesh.indices, 0, indexDataSize, indices);

    written += stream->write(vertices, vertexDataSize);
    written += stream->write(indices, indexDataSize);

    delete [] indices;
    delete [] vertices;

    return written == sizeof(header) + attribDataSize + vertexDataSize + indexDataSize;
}

} // ns geom
} // ns gfx
} // ns mcr
