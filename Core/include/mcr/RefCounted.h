#pragma once

#include <new>
#include <mcr/Types.h>

namespace mcr {

template <typename T>
class rcptr
{
public:
    rcptr(): m_ptr(nullptr) {}

    rcptr(T* ptr): m_ptr(ptr)
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

    ~rcptr() // inherit not
    {
        if (m_ptr)
            m_ptr->drop();
    }


    template <typename R>
    operator rcptr<R>() const
    {
        return rcptr<R>(static_cast<R*>(m_ptr));
    }

    template <typename R>
    operator rcptr<R>()
    {
        return rcptr<R>(static_cast<R*>(m_ptr));
    }

    operator T*() const
    {
        return m_ptr;
    }

    T* operator->() const
    {
        return m_ptr;
    }


    rcptr<T>& operator=(const rcptr<T>& rhs)
    {
        return *this = static_cast<T*>(rhs);
    }

    rcptr<T>& operator=(T* rhs)
    {
        if (m_ptr)
            m_ptr->drop();

        if ((m_ptr = rhs))
            m_ptr->grab();

        return *this;
    }

private:
    T* m_ptr;
};

class RefCounted
{
public:
    RefCounted(uint numRefs = 0u): m_numRefs(numRefs) {}
    virtual ~RefCounted() {}

    void grab()
    {
        ++m_numRefs;
    }

    bool drop()
    {
        if (m_numRefs && --m_numRefs)
            return false;

        delete this;
        return true;
    }

protected:
    template <typename T> friend class rcptr;

    void* operator new(std::size_t size);
    void* operator new(std::size_t size, const std::nothrow_t& nothrowValue) throw();

    void operator delete(void* ptr) throw();
    void operator delete(void* ptr, const std::nothrow_t& nothrowValue) throw();

    uint m_numRefs;
};


inline void* RefCounted::operator new(std::size_t size)
{
    return ::operator new(size);
}

inline void* RefCounted::operator new(std::size_t size, const std::nothrow_t& nothrowValue) throw()
{
    return ::operator new(size, nothrowValue);
}

inline void RefCounted::operator delete(void* ptr) throw()
{
    ::operator delete(ptr);
}

inline void RefCounted::operator delete(void* ptr, const std::nothrow_t& nothrowValue) throw()
{
    ::operator delete(ptr, nothrowValue);
}

} // ns mcr
