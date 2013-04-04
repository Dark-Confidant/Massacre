#pragma once

#include <type_traits>

namespace mcr {

template <typename>
struct DataMemberTraits;

template <class Class, typename T>
struct DataMemberTraits<T Class::*>
{
    typedef Class Class;
    typedef T Type;
};


template <typename>
struct GetterTraits;

template <class Class, typename T>
struct GetterTraits<T(Class::*)()>
{
    typedef Class Class;
    typedef T ResultOptRef;
    typedef typename std::remove_const<typename std::remove_reference<ResultOptRef>::type>::type Result;
};

template <class Class, typename T>
struct GetterTraits<T(Class::*)() const>: GetterTraits<T(Class::*)()> {};


template <typename>
struct SetterTraits;

template <class Class, typename T, typename R>
struct SetterTraits<R(Class::*)(T)>
{
    typedef Class Class;
    typedef T ArgOptRef;
    typedef typename std::remove_const<typename std::remove_reference<ArgOptRef>::type>::type Arg;
};

template <class Class, typename T, typename R>
struct SetterTraits<R(Class::*)(T) const>: SetterTraits<R(Class::*)(T)> {};


template <typename>
struct FnTraits;

template <typename T>
struct FnTraits<T(*)()>
{
    typedef T ResultOptRef;
    typedef typename std::remove_const<typename std::remove_reference<ResultOptRef>::type>::type Result;
};

template <typename T, typename A0>
struct FnTraits<T(*)(A0)>: public FnTraits<T(*)()>
{
    typedef A0 Arg0OptRef;
    typedef typename std::remove_const<typename std::remove_reference<Arg0OptRef>::type>::type Arg0;
};

} // ns mcr
