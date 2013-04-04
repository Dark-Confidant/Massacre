#include "Universe.h"
#include "components/Hierarchy.h"
#include "Object.h"

using namespace mcr;

ComDesc Hierarchy::Desc = MCR_REGISTER_COMPONENT(Hierarchy);

void Hierarchy::onUpdate(int64 dt)
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
        (*it)->update(dt);
}

void Hierarchy::addChild(Object* obj)
{
    m_children.push_back(obj);

    if (obj->node()->m_parent)
        obj->node()->m_parent->node()->removeChild(obj);

    obj->node()->m_parent = m_obj;
}

void Hierarchy::removeChild(Object* obj)
{
    if (obj->node()->m_parent != m_obj)
        return;

    m_children.remove(obj);
    obj->node()->m_parent = nullptr;
}

void Hierarchy::removeFromParent()
{
    if (m_parent)
        m_parent->node()->removeChild(m_obj);
}
