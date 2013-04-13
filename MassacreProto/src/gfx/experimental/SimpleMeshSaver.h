#pragma once

#include <gfx/experimental/IMeshSaver.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

class SimpleMeshSaver: public IMeshSaver
{
public:
    bool save(const Mesh& mesh, const char* filename) const;
};

} // ns experimental
} // ns gfx
} // ns mcr
