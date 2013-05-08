#include "Universe.h"
#include <mcr/gfx/OldMeshes.h>

#include <map>
#include <SimpleMesh3.h>

namespace mcr {
namespace gfx {

rcptr<VertexArray> OldMeshes::load(
    IFile* file,
    std::vector<Mesh>& meshesOut,
    std::vector<std::string>* texNamesOut /*= nullptr*/)
{
    SimpleMesh::Header header;
    file->read(header);

    if (!header.checkSignature() || header.version != SIMPLE_MESH_VERSION)
        return nullptr;


    const uint
        vertexDataSize = header.vertexSize * header.numVertices,
        indexDataSize  = sizeof(uint)      * header.numIndices;

    byte* srcVertices = new byte[vertexDataSize];
    uint* srcIndices  = new uint[header.numIndices];

    std::vector<byte> dstVertices;
    std::vector<uint> dstIndices;

    dstVertices.reserve(vertexDataSize);
    dstIndices.reserve(header.numIndices);


    // read attributes

    VertexFormat fmt;

    auto attribs = new SimpleMesh::VertexAttribute[header.numAttributes];

    file->seek(header.attributeDataOffset);
    file->read(attribs, header.numAttributes);

    for (uint i = 0; i < header.numAttributes; ++i)
        fmt.addAttrib(GType(GType::SByte + attribs[i].type), attribs[i].length); // sic!

    delete [] attribs;


    // read vertices
    file->seek(header.vertexDataOffset);
    file->read(srcVertices, vertexDataSize);

    // read indices
    file->seek(header.indexDataOffset);
    file->read(srcIndices, header.numIndices);


    // read parts
    file->seek(header.partDataOffset);

    for (auto n = header.numParts; n--;)
    {
        SimpleMesh::Part part;
        file->read(part);

        Mesh mesh;

        mesh.primitiveType = PrimitiveType::Triangles;
        mesh.startVertex   = uint(dstVertices.size() / header.vertexSize);
        mesh.startIndex    = uint(dstIndices.size());

        std::map<uint, uint> done;

        for (uint i = part.start; i < part.start + part.size; ++i)
        {
            uint idx = srcIndices[i];

            auto it = done.find(idx);
            if (it != done.end())
            {
                dstIndices.push_back(it->second);
                continue;
            }

            uint newIdx = uint(dstVertices.size() / header.vertexSize);

            byte* vert = &srcVertices[idx * header.vertexSize];
            dstVertices.insert(dstVertices.end(), vert, vert + header.vertexSize);

            done.insert(std::make_pair(idx, newIdx));
            dstIndices.push_back(newIdx);
        }

        mesh.numVertices = uint(dstVertices.size() / header.vertexSize) - mesh.startVertex;
        mesh.numIndices  = uint(dstIndices.size()) - mesh.startIndex;

        meshesOut.push_back(mesh);


        if (!part.numTextures && texNamesOut)
            texNamesOut->push_back("");
        else
        {
            for (byte i = 0; i < part.numTextures; ++i)
            {
                std::string texName;
                file->readString0(texName);

                if (i == 0 && texNamesOut)
                    texNamesOut->push_back(texName);
            }
        }
    }

    delete [] srcVertices;
    delete [] srcIndices;


    auto buffer = VertexArray::create(
        &dstVertices[0], (uint) dstVertices.size(),
        &dstIndices[0], (uint) dstIndices.size() * sizeof(uint));

    buffer->setFormat(fmt);

    static auto flipUV = math::buildTransform(vec3(0, 1, 0), vec3(), vec3(1, -1, 0));
    buffer->transformAttribs(2, flipUV);

    for (uint i = 0; i < meshesOut.size(); ++i)
        meshesOut[i].buffer = buffer;

    return buffer;
}

} // ns gfx
} // ns mcr
