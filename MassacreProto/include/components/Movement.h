#pragma once

#include <deque>
#include "ComRegistry.h"

namespace mcr {

class Movement: public Component
{
public:
    static ComDesc Desc;

    static rcptr<Movement> create(Object* obj) { return new Movement(obj); }

    static void enumFields(FieldTree& fields)
    {
        fields.add("flags", &Movement::flags, &Movement::setFlags);
        fields.add("face", &Movement::face, &Movement::setFace);
        fields.add("velocity", &Movement::velocity, &Movement::setVelocity);

        auto& info = fields.addNode("info");
        info.add("moveVector", &Movement::moveVector);
        info.add("direction", &Movement::direction);
    }


    void onUpdate(int64 dt);


    vec3 moveVector() const
    {
        return math::normalize(vec3(
            0.f + !!(m_flags & 8) - !!(m_flags & 4), 0,
            0.f + !!(m_flags & 2) - !!(m_flags & 1)));
    }

    vec3 direction() const
    {
        auto mv = moveVector();

        auto yAngle = math::deg2rad * m_face.y();
        auto ySine = sin(yAngle), yCosine = cos(yAngle);

        return
            mv.x() * vec3(yCosine, 0, -ySine) +
            mv.z() * vec3(ySine, 0, yCosine);
    }


    uint flags()       const { return m_flags; }
    const vec2& face() const { return m_face; }
    float velocity()   const { return m_velocity; }

    void setFlags(uint flags)        { m_flags = flags; }
    void setFace(const vec2& face)   { m_face = face; }
    void setVelocity(float velocity) { m_velocity = velocity; }

protected:
    Movement(Object* obj):
    Component(obj),
        m_flags(0),
        m_velocity(1.f) {}

    ~Movement() {}

    uint m_flags;
    vec2 m_face;
    float m_velocity;
};

} // ns mcr
