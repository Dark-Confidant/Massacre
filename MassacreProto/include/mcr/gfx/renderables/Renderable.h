#pragma once

#include <mcr/RefCounted.h>
#include <mcr/gfx/RenderAtom.h>

namespace mcr {
namespace gfx {

class Renderable: public RefCounted
{
public:
    virtual uint numAtoms() const = 0;
    virtual const RenderAtom* atom(uint idx) const = 0;
};

}
}
