#include "Universe.h"
#include "gfx/renderables/Renderable.h"

using namespace mcr;
using namespace gfx;

const Event
    Renderable::AtomAdded,
    Renderable::AtomRemoved,
    Renderable::AtomInvalidated;

const EventArray Renderable::Events(
    Renderable::AtomAdded,
    Renderable::AtomRemoved,
    Renderable::AtomInvalidated);
