#pragma once

#include "Object.h"

namespace mcr {

class WorldAction
{
public:
    ~WorldAction() {}

    virtual void execute() = 0;
    virtual void cancel() = 0;
};

class SetMovement: public WorldAction
{
public:
    SetMovement(uint objId, uint flags, const vec2& face):
        m_objId(objId), m_flags(flags), m_face(face) {}

    void execute()
    {
        m_obj = Object::find(m_objId);

        m_prevFlags = obj->movement()->flags();
        m_prevFace  = obj->movement()->face();

        obj->movement()->setFlags(m_flags);
        obj->movement()->setFace(m_face);
    }

    void cancel()
    {
        obj->movement()->setFlags(m_prevFlags);
        obj->movement()->setFace(m_prevFace);
    }

protected:
    uint m_objId;
    rcptr<Object> m_obj;
    uint m_flags, m_prevFlags;
    vec2 m_face, m_prevFace;
};

class Spawn: public WorldAction
{
public:
    Spawn(uint parentId, const vec3& pos):
        m_parentId(parentId), m_pos(pos) {}

    void execute()
    {
        if (!m_obj)
            m_obj = Object::create(Object::find(m_parentId));
        else
            m_obj->removeComponent<Ghost>();

        m_obj->transform()->setPosition(m_pos);
    }

    void cancel()
    {
        m_obj->addComponent<Ghost>();
    }

protected:
    uint m_parentId;
    rcptr<Object> m_obj;
    vec3 m_pos;
};

class Withdraw: public WorldAction
{
public:
    Withdraw(uint objId): m_objId(objId) {}

    void execute()
    {
        if (!m_obj)
            m_obj = Object::find(m_objId);

        m_obj->addComponent<Ghost>();
    }

    void cancel()
    {
        m_obj->removeComponent<Ghost>();
    }

protected:
    uint m_objId;
    rcptr<Object> m_obj;
};

} // namespace mcr
