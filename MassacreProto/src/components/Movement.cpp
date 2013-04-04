#include "Universe.h"
#include "components/Movement.h"
#include "Object.h"

using namespace mcr;

ComDesc Movement::Desc = MCR_REGISTER_COMPONENT(Movement);

void Movement::onUpdate(int64 dt)
{
    auto dtSec = .001f * dt;

    auto tf = m_obj->transform();
    tf->setPosition(tf->position() + dtSec * m_velocity * direction());
}
