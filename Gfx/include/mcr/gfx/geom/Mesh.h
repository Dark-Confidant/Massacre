#pragma once

#include <mcr/GfxExtern.h>
#include <mcr/io/IFileReader.h>
#include <mcr/io/IWriter.h>
#include <mcr/gfx/geom/VertexFormat.h>
#include <mcr/gfx/geom/mem/IVideoMemory.h>

namespace mcr  {
namespace gfx  {
namespace geom {

struct PrimitiveType
{
    enum Type
    {
        Point,
        Triangles,
        TriangleStrip,
        TriangleFan,
        Quads
    }
    type;

    PrimitiveType(): type(Triangles) {}
    PrimitiveType(Type atype): type(atype) {}
    explicit PrimitiveType(uint atype): type(Type(atype)) {}

    operator Type() const { return type; }
};

struct Mesh
{
    VertexFormat        vertexFormat;
    PrimitiveType       primitiveType;

    mem::VideoHandle    vertices;
    uint                numVertices;

    mem::VideoHandle    indices;
    uint                numIndices;

    MCR_GFX_EXTERN static bool load(
        io::IFileReader* stream,
        mem::IVideoMemory* vertMem,
        mem::IVideoMemory* idxMem,
        Mesh& meshOut);

    MCR_GFX_EXTERN static bool save(
        io::IWriter* stream,
        const Mesh& mesh);
};

} // ns geom
} // ns gfx
} // ns mcr