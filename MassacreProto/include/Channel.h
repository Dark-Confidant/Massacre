#pragma once

#include "FnTraits.h"

namespace mcr {

#define MCR_TYPEVAL(x) decltype(x), x

// OUTPUT

template <typename T>
static void outputValueStrategy(void* object, const T& value)
{
    *static_cast<T*>(object) = value;
}

template <typename Fn, Fn fn>
static void outputFnStrategy(void*, const typename FnTraits<Fn>::Arg0& value)
{
    fn(value);
}

template <typename Setter, Setter setter>
static void outputMethodStrategy(void* object, const typename SetterTraits<Setter>::Arg& value)
{
    (static_cast<typename SetterTraits<Setter>::Class*>(object)->*setter)(value);
}


template <typename T>
class OutputChannel
{
public:
    typedef void(*StrategyFn)(void*, const T&);

    OutputChannel<T>(): m_object(), m_strategy() {}
    OutputChannel<T>(T* object): m_object(object), m_strategy(&outputValueStrategy<T>) {}
    OutputChannel<T>(void* object, StrategyFn strategy): m_object(object), m_strategy(strategy) {}
    ~OutputChannel<T>() {}

    OutputChannel<T>& operator<<(const T& value)
    {
        m_strategy(m_object, value);
        return *this;
    }

private:
    void* m_object;
    StrategyFn m_strategy;
};


template <typename T>
inline OutputChannel<T> outChannel(T* object)
{
    return OutputChannel<T>(object);
}

template <typename T>
inline OutputChannel<T> outChannel(T& object)
{
    return OutputChannel<T>(&object);
}

template <typename Fn, Fn fn>
inline OutputChannel<typename FnTraits<Fn>::Arg0> outChannel()
{
    return OutputChannel<typename FnTraits<Fn>::Arg0>(nullptr, outputFnStrategy<Fn, fn>);
}

template <typename Setter, Setter setter>
inline OutputChannel<typename SetterTraits<Setter>::Arg> outChannel(typename SetterTraits<Setter>::Class* object)
{
    return OutputChannel<typename SetterTraits<Setter>::Arg>(object, outputMethodStrategy<Setter, setter>);
}


// INPUT

template <typename T>
static void inputValueStrategy(const void* object, T& value)
{
    value = *static_cast<const T*>(object);
}

template <typename Fn, Fn fn>
static void inputFnStrategy(const void*, typename FnTraits<Fn>::Result& value)
{
    value = fn();
}

template <typename Getter, Getter getter>
static void inputMethodStrategy(const void* object, typename GetterTraits<Getter>::Result& value)
{
    value = (static_cast<const typename GetterTraits<Getter>::Class*>(object)->*getter)();
}


template <typename T>
class InputChannel
{
public:
    typedef void(*StrategyFn)(const void*, T&);

    InputChannel<T>(): m_object(), m_strategy() {}
    InputChannel<T>(const T* object): m_object(object), m_strategy(&inputValueStrategy<T>) {}
    InputChannel<T>(const void* object, StrategyFn strategy): m_object(object), m_strategy(strategy) {}
    ~InputChannel<T>() {}

    InputChannel<T>& operator>>(T& value)
    {
        m_strategy(m_object, value);
        return *this;
    }

private:
    const void* m_object;
    StrategyFn m_strategy;
};


template <typename T>
inline InputChannel<T> inChannel(const T* object)
{
    return InputChannel<T>(object);
}

template <typename T>
inline InputChannel<T> inChannel(const T& object)
{
    return InputChannel<T>(&object);
}

template <typename Fn, Fn fn>
inline InputChannel<typename FnTraits<Fn>::Result> inChannel()
{
    return InputChannel<typename FnTraits<Fn>::Result>(nullptr, inputFnStrategy<Fn, fn>);
}

template <typename Getter, Getter getter>
inline InputChannel<typename GetterTraits<Getter>::Result> inChannel(const typename GetterTraits<Getter>::Class* object)
{
    return InputChannel<typename GetterTraits<Getter>::Result>(object, inputMethodStrategy<Getter, getter>);
}

} // ns mcr
