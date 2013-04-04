#include "Universe.h"
#include "components/Model.h"

#include "Object.h"

using namespace mcr;

ComDesc Model::Desc = MCR_REGISTER_COMPONENT(Model);
MCR_COMPONENT_ORDER(Render, Model);

void Model::setMesh(gfx::Mesh* mesh)
{
    if (m_mesh)
        m_mesh->removeListener(gfx::Renderable::Events, this);

    m_mesh = mesh;
    m_mesh->addListener(gfx::Renderable::Events, this);
}

void Model::onEvent(EventId event, const EventParams& params)
{
    m_obj->notify(event, params);
}
