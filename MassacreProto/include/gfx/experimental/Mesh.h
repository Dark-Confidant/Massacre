#pragma once

#include <RefCounted.h>
#include <gfx/VertexArray.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

/* Approach summary:
    - Bind material
    - Bind mesh storage
    - Draw meshes
*/

//class MeshStorage;

struct Mesh: public RefCounted
{
    //MeshStorage* storage;
    rcptr<VertexArray> buffer;
    uint startVertex, numVertices;
    uint startIndex, numIndices;
    uint primitiveType;

    const void* mapVertices() const;
    void*       mapVertices(uint access);
    void        unmapVertices() const;

    const uint* mapIndices() const;
    uint*       mapIndices(uint access);
    void        unmapIndices() const;
};


inline const void* Mesh::mapVertices() const
{
    const GBuffer* vertices = buffer->vertices();
    return vertices->map(startVertex, numVertices, 1); // TODO: enums
}

inline void* Mesh::mapVertices(uint access)
{
    return buffer->vertices()->map(startVertex, numVertices, access);
}

inline void Mesh::unmapVertices() const
{
    buffer->vertices()->unmap();
}


inline const uint* Mesh::mapIndices() const
{
    const GBuffer* indices = buffer->indices();
    return static_cast<const uint*>(indices->map(startIndex, numIndices, 1)); // TODO: enums
}

inline uint* Mesh::mapIndices(uint access)
{
    return static_cast<uint*>(buffer->indices()->map(access));
}

inline void Mesh::unmapIndices() const
{
    buffer->indices()->unmap();
}

} // ns experimental
} // ns gfx
} // ns mcr
