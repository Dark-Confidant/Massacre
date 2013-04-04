#pragma once

#include "math/Matrix.h"
#include "GBuffer.h"
#include "VertexFormat.h"

namespace mcr {
namespace gfx {

// TODO: make it PrimitiveArray maybe?
class VertexArray: public RefCounted
{
public:
    //! Create non-initialized vertex array
    static rcptr<VertexArray> create();

    //! Create vertex array of given sizes
    static rcptr<VertexArray> create(uint vertexDataSize, uint indexDataSize);

    //! Create vertex array and fill its buffers
    static rcptr<VertexArray> create(
        const void* vertexData, uint vertexDataSize,
        const void* indexData, uint indexDataSize);


    //! OpenGL VAO
    uint handle() const { return m_handle; }


    //! Vertex format
    const VertexFormat& format() const { return m_format; }

    //! Set vertex format
    void setFormat(const VertexFormat& fmt);


    //! Vertex buffer
    GBuffer* vertices() const { return m_vertices; }

    //! Index buffer
    GBuffer* indices() const { return m_indices; }


    //! Apply transform to particular attribute of all vertices of given range
    //! Note: only works for vecn and float attributes
    void transformAttribs(uint attribIdx, const mat4& tf, uint startVertex, uint numVertices);

    //! Similar to transformAttribs/4, but transforms the whole buffer
    void transformAttribs(uint attribIdx, const mat4& tf);

    // TODO: document
    void transformAttribsIndexed(uint attribIdx, const mat4& tf, uint startIndex, uint numIndices);

protected:
    VertexArray();
    ~VertexArray();

    uint m_handle;
    VertexFormat m_format;

    rcptr<GBuffer> m_vertices, m_indices;
};

} // ns gfx
} // ns mcr
