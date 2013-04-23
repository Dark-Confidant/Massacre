#pragma once

#include <mcr/MassacreTypes.h>

namespace mcr {

class RefCounted
{
public:
    RefCounted(uint numRefs = 0u):
        m_numRefs(numRefs) {}

    virtual ~RefCounted() {}


    void grab()
    { ++m_numRefs; }

    bool drop()
    {
        if (m_numRefs && --m_numRefs)
            return false;

        delete this;
        return true;
    }

protected:
    uint m_numRefs;
};


template <typename T>
class rcptr
{
public:
    rcptr():
        m_ptr(nullptr) {}

    rcptr(T* ptr):
        m_ptr(ptr)
    {
        if (m_ptr)
            m_ptr->grab();
    }

    rcptr(const rcptr<T>& other):
        m_ptr(other)
    {
        if (m_ptr)
            m_ptr->grab();
    }

    ~rcptr()
    {
        if (m_ptr)
            m_ptr->drop();
    }


    template <typename R>
    operator rcptr<R>() const
    { return rcptr<R>(static_cast<R*>(m_ptr)); }

    template <typename R>
    operator rcptr<R>()
    { return rcptr<R>(static_cast<R*>(m_ptr)); }

    operator T*() const
    { return m_ptr; }

    T* operator->() const
    { return m_ptr; }


    rcptr<T>& operator=(const rcptr<T>& rhs)
    { return *this = static_cast<T*>(rhs); }

    rcptr<T>& operator=(T* rhs)
    {
        if (m_ptr)
            m_ptr->drop();

        if ((m_ptr = rhs))
            m_ptr->grab();

        return *this;
    }

protected:
    T* m_ptr;
};

template <typename T>
class rcproxy
{
public:
    rcproxy(T* ptr):
        m_ptr(ptr) {}

    ~rcproxy() {}


    rcptr<T> ptr() const
    { return m_ptr; }

    operator rcptr<T>() const
    { return m_ptr; }

    T* operator->() const
    { return m_ptr; }

protected:
    T* m_ptr;
};

} // ns mcr
