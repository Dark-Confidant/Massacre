#pragma once

#include <mcr/io/IFile.h>
#include <mcr/gfx/geom/Geometry.h>

namespace mcr  {
namespace gfx  {
namespace geom {

class IGeometryImporter: public RefCounted
{
public:
    virtual bool import(io::IFile* file, Geometry& geometryOut) = 0;
};

} // ns geom
} // ns gfx
} // ns mcr
