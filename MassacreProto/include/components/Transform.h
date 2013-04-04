#pragma once

#include "ComRegistry.h"
#include "math/Matrix.h"

namespace mcr {

class Transform: public Component
{
public:
    static ComDesc Desc;

    static rcptr<Transform> create(Object* obj) { return new Transform(obj); }

    static void enumFields(FieldTree& fields)
    {
        fields.add("transform", &Transform::local, &Transform::setLocal);

        auto& tf = fields.addNode("transform");
        tf.add("rotation", &Transform::rotation, &Transform::setRotation);
        tf.add("position", &Transform::position, &Transform::setPosition);

        auto& pos = tf.addNode("position");
        pos.add("x", &Transform::x, &Transform::setX);
        pos.add("y", &Transform::y, &Transform::setY);
        pos.add("z", &Transform::z, &Transform::setZ);

        auto& info = fields.addNode("info");
        info.add("direction", &Transform::direction);
        info.add("globalMatrix", &Transform::global);
    }


    const mat4& global() const
    {
        return m_global;
    }
    void updateGlobal();

    const mat4& local() const
    {
        return m_local;
    }
    void setLocal(const mat4& tf)
    {
        m_local = tf;
        updateGlobal();
    }

    vec3 direction() const
    {
        return vec3(0, 0, 1) * m_local;
    }

    const vec3& position() const
    {
        return m_local.translation();
    }
    float x() const
    {
        return m_local[12];
    }
    float y() const
    {
        return m_local[13];
    }
    float z() const
    {
        return m_local[14];
    }
    void setPosition(const vec3& pos)
    {
        m_local.setTranslation(pos);
        updateGlobal();
    }
    void setPosition(float x, float y, float z)
    {
        m_local.setTranslation(x, y, z);
        updateGlobal();
    }
    void setX(float x)
    {
        m_local[12] = x;
        updateGlobal();
    }
    void setY(float y)
    {
        m_local[13] = y;
        updateGlobal();
    }
    void setZ(float z)
    {
        m_local[14] = z;
        updateGlobal();
    }

    const vec3& rotation() const
    {
        return m_rotation;
    }
    void setRotation(const vec3& rot)
    {
        m_rotation = rot;
        rebuildRotation();
    }
    void setRotation(float x, float y, float z)
    {
        m_rotation.set(x, y, z);
        rebuildRotation();
    }

    const vec3& scale() const
    {
        return m_scale;
    }
    void setScale(const vec3& scl)
    {
        m_scale = scl;
        rebuildRotation();
    }
    void setScale(float x, float y, float z)
    {
        m_scale.set(x, y, z);
        rebuildRotation();
    }
    void setScale(float xyz)
    {
        m_scale.set(xyz, xyz, xyz);
        rebuildRotation();
    }

protected:
    Transform(Object* obj): Component(obj), m_scale(1) {}
    ~Transform() {}

    void onEvent(EventId, const EventParams&) {}

    void rebuildRotation()
    {
        m_local.setRotation(m_rotation);
        m_local.scaleUp(m_scale);

        updateGlobal();
    }

    vec3 m_rotation, m_scale;
    mat4 m_local, m_global;
};

} // ns mcr
