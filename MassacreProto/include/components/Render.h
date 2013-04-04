#pragma once

#include <unordered_map>
#include "ComRegistry.h"
#include "gfx/RenderAtom.h"

namespace mcr {
namespace gfx {

struct RenderData;
class Renderer;

} // ns gfx

class Render: public Component
{
public:
    static ComDesc Desc;

    static rcptr<Render> create(Object* obj) { return new Render(obj); }
    static void enumFields(FieldTree& /*fields*/) {}

    void onUpdate(int64);

    gfx::Renderer* renderer() const { return m_renderer; }
    void setRenderer(gfx::Renderer*);

protected:
    Render(Object* obj);
    ~Render();

    void onEvent(EventId event, const EventParams& params);

    void deregisterAtoms();

    gfx::Renderer* m_renderer;
    std::unordered_map<const gfx::RenderAtom*, const gfx::RenderData*> m_atoms;
    std::unordered_set<const gfx::RenderAtom*> m_dirtyAtoms;
};

} // ns mcr
