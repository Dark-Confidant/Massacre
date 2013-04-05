#include "Universe.h"
#include "components/Transform.h"
#include "components/Hierarchy.h"
#include "Object.h"

using namespace mcr;

ComDesc Transform::Desc = MCR_REGISTER_COMPONENT(Transform);
MCR_COMPONENT_ORDER(Hierarchy, Transform);

void Transform::updateGlobal()
{
    if (auto parent = m_obj->node()->parent())
        m_global = parent->transform()->global() * m_local;
    else
        m_global = m_local;

    auto& children = m_obj->node()->children();

    // consider using BOOST_FOREACH
    for (auto it = children.begin(); it != children.end(); ++it)
        (*it)->transform()->updateGlobal();
}
