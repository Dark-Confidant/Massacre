#pragma once

#include <vector>
#include <mcr/math/Vector.h>

namespace mcr  {
namespace gfx  {
namespace geom {

struct Triangle
{
    struct Vertex
    {
        std::size_t
            position,
            normal,     //< same as binormal/tangent, if present
            uv,
            color;
    }
    v[3];
};

struct Geometry
{
    std::vector<vec3> positions, normals, binormals, tangents;
    std::vector<std::vector<vec2>> uvs;     //< sub-vectors must be of the same length
    std::vector<std::vector<vec4>> colors;  //< \see uvs
    std::vector<Triangle> tris;
};

} // ns geom
} // ns gfx
} // ns mcr
