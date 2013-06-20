#pragma once

#include <mcr/gfx/geom/Mesh.h>
#include <mcr/gfx/geom/mem/NaiveMemory.h>

namespace mcr  {
namespace gfx  {
namespace geom {

class MeshManager
{
public:
    MeshManager();

    bool loadDynamic(io::IFileReader* stream, Mesh& meshOut);
    bool loadStatic(io::IFileReader* stream, Mesh& meshOut);

    mem::NaiveMemory& dynamicVertexMem();
    mem::NaiveMemory& dynamicIndexMem();
    mem::NaiveMemory& staticVertexMem();
    mem::NaiveMemory& staticIndexMem();

private:
    mem::NaiveMemory
        m_dynamicVertices,
        m_dynamicIndices,
        m_staticVertices,
        m_staticIndices;
};


inline MeshManager::MeshManager():
    m_dynamicVertices(mem::NaiveMemory::Vertex, mem::NaiveMemory::Dynamic),
    m_dynamicIndices (mem::NaiveMemory::Index,  mem::NaiveMemory::Dynamic),
    m_staticVertices (mem::NaiveMemory::Vertex, mem::NaiveMemory::Static),
    m_staticIndices  (mem::NaiveMemory::Index,  mem::NaiveMemory::Static) {}

inline bool MeshManager::loadDynamic(io::IFileReader* stream, Mesh& meshOut)
{
    return Mesh::load(stream, &m_dynamicVertices, &m_dynamicIndices, meshOut);
}

inline bool MeshManager::loadStatic(io::IFileReader* stream, Mesh& meshOut)
{
    return Mesh::load(stream, &m_staticVertices, &m_staticIndices, meshOut);
}

inline mem::NaiveMemory& MeshManager::dynamicVertexMem()
{
    return m_dynamicVertices;
}

inline mem::NaiveMemory& MeshManager::dynamicIndexMem()
{
    return m_dynamicIndices;
}

inline mem::NaiveMemory& MeshManager::staticVertexMem()
{
    return m_staticVertices;
}

inline mem::NaiveMemory& MeshManager::staticIndexMem()
{
    return m_staticIndices;
}

} // ns geom
} // ns gfx
} // ns mcr
