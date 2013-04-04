#pragma once
#pragma pack(push, 1)

#define SIMPLE_MESH_VERSION 4

namespace SimpleMesh {

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
    unsigned int   partDataOffset, numParts;

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

struct Part
{
    unsigned int start, size;
    // null-terminated hint string
};

} // ns SimpleMesh

#pragma pack(pop)
