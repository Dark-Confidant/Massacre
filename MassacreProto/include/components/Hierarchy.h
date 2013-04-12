#pragma once

#include "ComRegistry.h"

namespace mcr {

class Hierarchy: public Component
{
public:
    typedef std::list<rcptr<Object>> ObjectList; // consider performance

    static ComDesc Desc;

    static rcptr<Hierarchy> create(Object* obj) { return new Hierarchy(obj); }
    static void enumFields(FieldTree& /*fields*/) {}

    void onUpdate(int64 dt);

    Object* parent() const { return m_parent; }
    const ObjectList& children() const { return m_children; }

    void addChild(Object* obj);
    void removeChild(Object* obj);
    void removeFromParent();

protected:
    Hierarchy(Object* obj): Component(obj), m_parent(nullptr) {}
    ~Hierarchy() {}

    Object* m_parent;
    ObjectList m_children;
};

} // ns mcr
