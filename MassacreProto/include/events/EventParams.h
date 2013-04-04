#pragma once

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>
#include <boost/preprocessor/repetition.hpp>

namespace mcr {

class EventParams: boost::noncopyable
{
public:

    EventParams(): m_count(0), m_params(nullptr) {}

#define EVENT_PARAMS_set(z, n, unused)                        \
    m_params[n] = p ## n; 

#define EVENT_PARAMS_ctr(z, n, unused)                        \
    template <BOOST_PP_ENUM_PARAMS(n, typename T)>            \
    EventParams(BOOST_PP_ENUM_BINARY_PARAMS(n, T, const& p)): \
        m_count(n), m_params(new boost::any[n])               \
    {                                                         \
        BOOST_PP_REPEAT(n, EVENT_PARAMS_set, ~)               \
    } 

    BOOST_PP_REPEAT_FROM_TO(1, 21, EVENT_PARAMS_ctr, ~)

#undef EVENT_PARAMS_ctr
#undef EVENT_PARAMS_set

    ~EventParams()
    {
        if (m_params)
            delete [] m_params;
    }

    const boost::any& operator[](int i) const
    {
        return m_params[i];
    }

    int count() const
    {
        return m_count;
    }

private:
    const int m_count;
    boost::any* m_params;
};

} // ns mcr
