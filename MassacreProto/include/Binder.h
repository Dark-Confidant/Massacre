#pragma once

#include <typeinfo>
#include <boost/any.hpp>
#include "RefCounted.h"
#include "FnTraits.h"

namespace mcr {

class Binder
{
public:
    class Value: public RefCounted
    {
    public:
        virtual const std::type_info& type() const = 0;

        virtual boost::any get() const = 0;
        virtual void set(const boost::any&) = 0;

    protected:
        friend class Binder;

        Value() {}
        virtual ~Value() {}
    };

    virtual ~Binder() {}

    virtual const std::type_info& objectType() const = 0;
    virtual const std::type_info& valueType()  const = 0;

    virtual rcptr<Value> bind(void* object) = 0;
};


template <typename T>
class TypedBinder: public Binder
{
public:
    class Value: public Binder::Value
    {
    public:
        const std::type_info& type() const { return typeid(T); }

        boost::any get()                  const { return getTypedCopy(); }
        void       set(const boost::any& value) { setTyped(boost::any_cast<T>(value)); }

        virtual T        getTypedCopy() const = 0;
        virtual const T& getTypedRef()  const = 0;
        virtual void     setTyped(const T&)   = 0;

    protected:
        friend class TypedBinder<T>;

        Value() {}
        ~Value() {}
    };

    const std::type_info& valueType() const { return typeid(T); }

    rcptr<Binder::Value> bind(void* object) { return bindTyped(object); }

    virtual rcptr<Value> bindTyped(void* object) = 0;
};


#define MCR_DATA_MEMBER_BINDER(dataMember) \
    mcr::DataMemberBinder<decltype(dataMember), dataMember>

template <
    typename DataMember, DataMember dataMember,
    typename Class = typename DataMemberTraits<DataMember>::Class,
    typename T     = typename DataMemberTraits<DataMember>::Type>
class DataMemberBinder: public TypedBinder<T>
{
public:
    class Value: public TypedBinder<T>::Value
    {
    public:
        T        getTypedCopy()     const { return m_instance->*dataMember;  }
        const T& getTypedRef()      const { return m_instance->*dataMember;  }
        void     setTyped(const T& value) { m_instance->*dataMember = value; }

    protected:
        friend class DataMemberBinder<DataMember, dataMember>;

        Value(Class* instance): m_instance(instance) {}
        ~Value() {}

    private:
        Class* m_instance;
    };

    const std::type_info& objectType() const { return typeid(Class); }
    const std::type_info& valueType()  const { return typeid(T);     }

    rcptr<TypedBinder<T>::Value> bindTyped(void* object)
    {
        return bindDataMember((Class*) object);
    }

    rcptr<Value> bindDataMember(Class* object)
    {
        return new Value(object);
    }
};


#define MCR_PROPERTY_BINDER(getter, setter) \
    mcr::PropertyBinder<decltype(getter), getter, decltype(setter), setter>

template <
    typename Getter, Getter getter,
    typename Setter, Setter setter,
    typename Class = typename GetterTraits<Getter>::Class,
    typename T     = typename GetterTraits<Getter>::Result>
class PropertyBinder: public TypedBinder<T>
{
public:
    class Value: public TypedBinder<T>::Value
    {
    public:
        T        getTypedCopy()     const { return (m_instance->*getter)();                             }
        const T& getTypedRef()      const { return _ref<typename GetterTraits<Getter>::ResultOptRef>(); }
        void     setTyped(const T& value) { (m_instance->*setter)(value);                               }

    protected:
        friend class PropertyBinder<Getter, getter, Setter, setter>;

        Value(Class* instance): m_instance(instance) {}
        ~Value() {}

    private:
        template <typename R>
        typename std::enable_if<std::is_reference<R>::value, const T&>::type _ref() const
        {
            return (m_instance->*getter)();
        }

        template <typename R>
        typename std::enable_if<!std::is_reference<R>::value, const T&>::type _ref() const
        {
            throw std::runtime_error("No reference getter");
        }

        Class* m_instance;
    };

    const std::type_info& objectType() const { return typeid(Class); }
    const std::type_info& valueType()  const { return typeid(T);     }

    rcptr<TypedBinder<T>::Value> bindTyped(void* object)
    {
        return bindProperty((Class*) object);
    }

    rcptr<Value> bindProperty(Class* object)
    {
        return new Value(object);
    }
};


static void test_binding()
{
    struct A
    {
        int m_a;
    };


    MCR_DATA_MEMBER_BINDER(&A::m_a) dataMemberBinder;

    TypedBinder<int>* typedBinder    = &dataMemberBinder;
    Binder*           abstractBinder = &dataMemberBinder;


    A a;

    auto typedBinding    = typedBinder   ->bindTyped(&a);
    auto abstractBinging = abstractBinder->bind(&a);


    typedBinding->setTyped(1);
    assert(typedBinding->getTypedRef() == 1);

    abstractBinging->set(2);
    assert(boost::any_cast<int>(abstractBinging->get()) == 2);

    assert(typedBinding->getTypedRef() == boost::any_cast<int>(abstractBinging->get()));


    assert(&abstractBinder->valueType()  == &abstractBinging->type());
    assert(&abstractBinder->objectType() == &typeid(A));
    assert(&abstractBinging->type()      == &typeid(int));
}

} // ns mcr
