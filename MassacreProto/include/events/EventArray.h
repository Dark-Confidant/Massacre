#pragma once

#include <cstring>
#include <boost/preprocessor/repetition.hpp>
#include "Event.h"

namespace mcr {

class EventArray
{
public:

#define EVENT_ARRAY_ctr(z, n, unused)               \
    EventArray(BOOST_PP_ENUM_PARAMS(n, EventId p)): \
        m_size(n), m_array(new EventId[n])          \
    {                                               \
        memcpy(m_array, &p0, n * sizeof(EventId));  \
    } 

    BOOST_PP_REPEAT_FROM_TO(1, 21, EVENT_ARRAY_ctr, ~)

#undef EVENT_ARRAY_ctr

    ~EventArray() { delete [] m_array; }

    EventId operator[](int i) const { return m_array[i]; }
    int size() const { return m_size; }

private:
    const int m_size;
    EventId* m_array;
};

} // ns mcr
