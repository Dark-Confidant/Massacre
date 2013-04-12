#pragma once

#include "RefCounted.h"

namespace mcr {

class Object;

class Component: public RefCounted
{
public:
    virtual void onUpdate(int64 /*dt*/) {}

protected:
    Component(Object* obj): m_obj(obj) {}
    ~Component() {}

    Object* m_obj;
};

} // ns mcr
