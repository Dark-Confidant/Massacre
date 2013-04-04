#pragma once

#include <map>
#include "events/EventTarget.h"
#include "components/Hierarchy.h"
#include "components/Transform.h"
#include "components/Movement.h"

namespace mcr {

class Object: public RefCounted, public EventTarget
{
public:
    static const uint NumComponents = 16;
    static const Event ComAdded, ComRemoved;
    static const EventArray Events;

    static rcptr<Object> create(Object* parent = nullptr)
    {
        rcptr<Object> obj = new Object;

        if (parent && parent->node())
            parent->node()->addChild(obj);

        return obj;
    }


    // object actions

    void update(int64 dt)
    {
        for (uint i = 0; i < m_activeComponents.size(); ++i)
            m_activeComponents[i]->onUpdate(dt);
    }


    // dynamic component management

    Component* addComponent(uint id)
    {
        auto& slot = m_components[id];
        if (!slot)
        {
            slot = ComRegistry::instance().createComponent(id, this);

            notify(ComAdded, id);

            m_activeComponents.push_back(slot);
        }

        return slot;
    }

    bool removeComponent(uint id)
    {
        auto& slot = m_components[id];
        if (!slot)
            return false;

        notify(ComRemoved, id);

        for (uint i = 0; i < m_activeComponents.size() - 1; ++i)
            if (m_activeComponents[i] == slot)
            {
                std::swap(m_activeComponents[i], m_activeComponents.back());
                break;
            }

        m_activeComponents.pop_back();
        m_components[id] = nullptr;

        return true;
    }

    const Component* com(uint id) const
    {
        return m_components[id];
    }

    Component* com(uint id)
    {
        return m_components[id];
    }


    // static component management

    template <class T>
    T* addComponent()
    {
        return (T*) addComponent(T::Desc.id);
    }
        
    template <class T>
    bool removeComponent()
    {
        return removeComponent(T::Desc.id);
    }

    template <class T>
    T* com()
    {
        return (T*) com(T::Desc.id);
    }

    template <class T>
    const T* com() const
    {
        return (const T*) com(T::Desc.id);
    }


    // common component shortcuts

    const Hierarchy* node()      const { return com<Hierarchy>(); }
    const Transform* transform() const { return com<Transform>(); }
    const Movement*  movement()  const { return com<Movement>();  }

    Hierarchy* node()      { return com<Hierarchy>(); }
    Transform* transform() { return com<Transform>(); }
    Movement*  movement()  { return com<Movement>();  }

protected:
    Object()
    {
        addComponent<Hierarchy>();
        addComponent<Transform>();
    }

    ~Object() {}

    std::vector<Component*> m_activeComponents;
    rcptr<Component> m_components[NumComponents];
};

} // ns mcr
