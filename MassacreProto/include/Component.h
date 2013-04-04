#pragma once

#include "RefCounted.h"
#include "events/EventListener.h"

namespace mcr {

class Object;

class Component: public RefCounted, public EventListener
{
public:
    virtual void onUpdate(int64 /*dt*/) {}

protected:
    Component(Object* obj): m_obj(obj) {}
    ~Component() {}

    Object* m_obj;
};

} // ns mcr
