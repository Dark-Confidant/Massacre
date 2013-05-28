#pragma once

#include <mcr/RefCounted.h>
#include <mcr/gfx/geom/Geometry.h>

namespace mcr  {
namespace gfx  {
namespace geom {

class IGeometryExporter: public RefCounted
{
public:
    virtual bool export_(const Geometry& geometry, const char* filename) const = 0;
};

} // ns geom
} // ns gfx
} // ns mcr
