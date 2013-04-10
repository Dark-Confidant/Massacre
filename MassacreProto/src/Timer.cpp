#include "Universe.h"
#include "Timer.h"

#ifdef MCR_PLATFORM_LINUX
#   include <ctime>
#endif

namespace mcr {
#if defined(MCR_PLATFORM_WINDOWS)

void Timer::initFrequency()
{
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
}

int64 Timer::queryTicks() const
{
    int64 result;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&result));

    return result;
}

#elif defined(MCR_PLATFORM_LINUX)

#   ifdef CLOCK_MONOTONIC_RAW
#       define MCR_TIMER_CLOCK_ID CLOCK_MONOTONIC_RAW
#   else
#       define MCR_TIMER_CLOCK_ID CLOCK_MONOTONIC
#   endif

void Timer::initFrequency()
{
    m_frequency = 1000000000ll; // nanoseconds
}

int64 Timer::queryTicks() const
{
    timespec ts;
    clock_gettime(MCR_TIMER_CLOCK_ID, &ts);

    return 1000000000ll * ts.tv_sec + ts.tv_nsec;
}

#   undef MCR_TIMER_CLOCK_ID

#endif
} // ns mcr
