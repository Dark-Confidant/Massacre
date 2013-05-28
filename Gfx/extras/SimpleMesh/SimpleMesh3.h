#pragma once
#pragma pack(push, 1)

#define SIMPLE_MESH_VERSION 3

namespace SimpleMesh
{
    struct Header
    {
        unsigned short signature;
        unsigned short version;
        unsigned int   flags;
        unsigned short vertexSize;
        unsigned int   attributeDataOffset, numAttributes;
        unsigned int   vertexDataOffset, numVertices;
        unsigned int   indexDataOffset, numIndices;
        unsigned int   partDataOffset, numParts;

        bool checkSignature() const
        { return signature == 0x4d53u; }
    };

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

    struct MaterialParams
    {
        unsigned char states;
        unsigned char alphaFunc;
        unsigned char depthFunc;
        unsigned char blendSrcFunc, blendDstFunc;
        float         alphaThreshold;

        bool getState(unsigned char state) const
        { return (states & state) != 0; }

        void setState(unsigned char state, bool enabled)
        { enabled ? states |= state : states &= ~state; }
    };

    typedef unsigned int Index;

    struct Part
    {
        Index start, size;
        MaterialParams material;
        unsigned char numTextures;
        // [texName as null-terminated string] times numTextures
    };

    namespace MaterialState
    {
        enum
        {
            AlphaTest  = 1 << 0,
            DepthTest  = 1 << 1,
            DepthWrite = 1 << 2,
            Blend      = 1 << 3,
            CullFace   = 1 << 4
        };
    }

    namespace MaterialFunc
    {
        enum
        {
            Never,
            Less,
            Equal,
            LEqual,
            Greater,
            NotEqual,
            GEqual,
            Always
        };
    }

    namespace MaterialBlendFunc
    {
        enum
        {
            SrcColor, OneMinusSrcColor,
            SrcAlpha, OneMinusSrcAlpha,
            DstAlpha, OneMinusDstAlpha,
            DstColor, OneMinusDstColor,
            SrcAlphaSaturate
        };
    }
}

#pragma pack(pop)
