#pragma once

#include <mcr/gfx/VertexArray.h>

namespace mcr {
namespace gfx {

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
    rcptr<VertexArray> buffer;
    uint startVertex, numVertices;
    uint startIndex, numIndices;
    PrimitiveType primitiveType;
};

} // ns gfx
} // ns mcr
