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

    bool loadDynamic(io::IFile* file, Mesh& meshOut);
    bool loadStatic(io::IFile* file, Mesh& meshOut);

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

inline bool MeshManager::loadDynamic(io::IFile* file, Mesh& meshOut)
{
    return Mesh::load(file, &m_dynamicVertices, &m_dynamicIndices, meshOut);
}

inline bool MeshManager::loadStatic(io::IFile* file, Mesh& meshOut)
{
    return Mesh::load(file, &m_staticVertices, &m_staticIndices, meshOut);
}

} // ns geom
} // ns gfx
} // ns mcr
