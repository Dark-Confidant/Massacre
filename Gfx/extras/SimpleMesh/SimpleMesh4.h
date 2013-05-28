#pragma once
#pragma pack(push, 1)

#define SIMPLE_MESH_VERSION 4

namespace SimpleMesh4 {

struct Header
{
    unsigned short signature;
    unsigned short version;
    unsigned int   flags;
    unsigned int   primitiveType;
    unsigned short vertexSize;
    unsigned int   attributeDataOffset, numAttributes;
    unsigned int   vertexDataOffset, numVertices;
    unsigned int   indexDataOffset, numIndices;

    bool checkSignature() const
    {
        return signature == 0x4d53u;
    }
};

namespace PrimitiveType {
enum
{
    Point,
    Triangle,
    TriangleStrip,
    TriangleFan,
    Quad
};
}

struct VertexAttribute
{
    enum
    {
        SByte, Byte,
        Short, UShort,
        Int,   UInt,
        Float, Double
    };

    unsigned char length, type;

    unsigned int getTypeSize() const
    {
        static const unsigned int sizes[] = {1, 1, 2, 2, 4, 4, 4, 8};
        return sizes[type];
    }
};

} // ns SimpleMesh4

#ifndef SIMPLE_MESH_STRICT_NS
namespace SimpleMesh = SimpleMesh4;
#endif

#pragma pack(pop)
