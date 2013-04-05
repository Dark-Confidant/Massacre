#include "Universe.h"
#include "components/Render.h"

#include "Object.h"
#include "gfx/Renderer.h"
#include "gfx/renderables/Renderable.h"

using namespace mcr;

ComDesc Render::Desc = MCR_REGISTER_COMPONENT(Render);

Render::Render(Object* obj):
Component(obj),
    m_renderer(nullptr)
{
    m_obj->addListener(gfx::Renderable::Events, this);
}

Render::~Render()
{
    deregisterAtoms();
}


void Render::onUpdate(int64)
{
    if (!m_renderer)
        return;

    // consider using BOOST_FOREACH
    for (auto it = m_dirtyAtoms.begin(); it != m_dirtyAtoms.end(); ++it)
    {
        auto& slot = m_atoms[*it];

        if (slot)
            m_renderer->deregisterData(slot);

        slot = m_renderer->registerData(m_obj, *it);
    }

    m_dirtyAtoms.clear();
}

void Render::setRenderer(gfx::Renderer* renderer)
{
    deregisterAtoms();

    m_renderer = renderer;

    if (m_renderer)
        BOOST_FOREACH (auto& pair, m_atoms)
            pair.second = m_renderer->registerData(m_obj, pair.first);
}

void Render::onEvent(EventId event, const EventParams& params)
{
    auto atom = boost::any_cast<const gfx::RenderAtom*>(params[0]);

    if (event == gfx::Renderable::AtomAdded)
    {
        auto& slot = m_atoms[atom];

        if (!slot && m_renderer)
            slot = m_renderer->registerData(m_obj, atom);
    }
    else if (event == gfx::Renderable::AtomRemoved)
    {
        auto it = m_atoms.find(atom);

        if (it != m_atoms.end())
        {
            if (m_renderer)
                m_renderer->deregisterData(it->second);

            m_atoms.erase(it);
            m_dirtyAtoms.erase(atom);
        }
    }
    else if (event == gfx::Renderable::AtomInvalidated)
    {
        if (m_renderer)
            m_dirtyAtoms.insert(atom);
    }
}

void Render::deregisterAtoms()
{
    if (!m_renderer)
        return;

    BOOST_FOREACH (auto& pair, m_atoms)
    {
        if (pair.second)
        {
            m_renderer->deregisterData(pair.second);
            pair.second = nullptr;
        }
    }

    m_dirtyAtoms.clear();
}
