#pragma once

#include <list>
#include <mcr/gfx/VertexArray.h>
#include <mcr/gfx/experimental/Mesh.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

class MeshStorage
{
protected:
    rcptr<VertexArray> m_buffer;
    std::list<rcptr<Mesh>> m_meshes;
};

} // ns experimental
} // ns gfx
} // ns mcr
