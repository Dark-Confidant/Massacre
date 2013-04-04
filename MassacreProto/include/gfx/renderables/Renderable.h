#pragma once

#include "RefCounted.h"
#include "gfx/RenderAtom.h"
#include "events/EventTarget.h"

namespace mcr {
namespace gfx {

class Renderable: public RefCounted, public EventTarget
{
public:
    static const Event AtomAdded, AtomRemoved, AtomInvalidated;
    static const EventArray Events;

    virtual uint numAtoms() const = 0;
    virtual const RenderAtom* atom(uint idx) const = 0;

protected:
    void onAddListener(EventId event, EventListener* listener)
    {
        if (event == AtomAdded)
            for (uint i = 0, n = numAtoms(); i != n; ++i)
                listener->onEvent(AtomAdded, atom(i));
    }

    void onRemoveListener(EventId event, EventListener* listener)
    {
        if (event == AtomRemoved)
            for (uint i = 0, n = numAtoms(); i != n; ++i)
                listener->onEvent(AtomRemoved, atom(i));
    }
};

}
}
