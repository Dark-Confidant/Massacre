#pragma once

#include <mcr/RefCounted.h>
#include <mcr/gfx/VertexFormat.h>

namespace mcr {
namespace gfx {

class GeometryBuffer;

struct Geometry
{
    GeometryBuffer* buffer;
    uint startVertex, numVertices;
    uint startIndex,  numIndices;
};

class GeometryBuffer: public RefCounted
{
public:
    static rcptr<GeometryBuffer> create(const VertexFormat& format, uint maxVertices, uint maxIndices);

    const Geometry* allocate(uint numVertices, uint numIndices);
    void            free(const Geometry* geom);

    const void*     mapVertices  (const Geometry* geom) const;
    void*           mapVertices  (const Geometry* geom);
    void            unmapVertices(const Geometry* geom) const;

    const uint*     mapIndices   (const Geometry* geom) const;
    uint*           mapIndices   (const Geometry* geom);
    void            unmapIndices (const Geometry* geom) const;

protected:
    // TODO: static bindings?

    GeometryBuffer(const VertexFormat& format, uint maxVertices, uint maxIndices);
    ~GeometryBuffer();

    VertexFormat m_format;
    uint m_maxVertices, m_maxIndices;

    uint m_handle;
};

} // ns gfx
} // ns mcr
