#pragma once

#include <mcr/gfx/experimental/IMeshExporter.h>

namespace mcr          {
namespace gfx          {
namespace experimental {

class SimpleMeshExporter: public IMeshExporter
{
public:
    using RefCounted::operator new;
    using RefCounted::operator delete;

    bool export_(const Mesh& mesh, const char* filename) const;
};

} // ns experimental
} // ns gfx
} // ns mcr
