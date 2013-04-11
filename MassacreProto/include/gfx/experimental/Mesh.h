#pragma once

#include "RefCounted.h"

namespace mcr          {
namespace gfx          {
namespace experimental {

/* Approach summary:
    - Bind material
    - Bind mesh storage
    - Draw meshes
*/

class MeshStorage;

struct Mesh: public RefCounted
{
    MeshStorage* storage;
    uint startIndex, numIndices;
    uint primitiveType;
};

} // ns experimental
} // ns gfx
} // ns mcr
