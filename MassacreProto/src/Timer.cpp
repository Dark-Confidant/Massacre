#include "Universe.h"
#include <mcr/Timer.h>

#if defined(MCR_PLATFORM_MAC)
#   include <mach/clock.h>
#elif defined(MCR_PLATFORM_LINUX)
#   include <ctime>
#endif

namespace mcr {
#if defined(MCR_PLATFORM_WINDOWS)

struct Timer::Impl
{
    int64 start;
};

void Timer::_initFrequency()
{
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
}

void Timer::_start()
{
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_impl.start));
}

int64 Timer::_ticksSinceStart() const
{
    int64 result;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&result));

    return result - m_impl.start;
}

#elif defined(MCR_PLATFORM_MAC)

struct Timer::Impl
{
    uint startSec, startMsec;
};

void Timer::_initFrequency()
{
    m_frequency = 1000000ll; // microseconds
}

void Timer::_start()
{
    clock_get_system_microtime(&m_impl.startSec, &m_impl.startMsec);
}

void Timer::_ticksSinceStart()
{
    uint sec, msec;
    clock_get_system_microtime(&sec, &msec);

    return 1000000ll * (sec - m_impl.startSec) + (msec - m_impl.startMsec);
}

#elif defined(MCR_PLATFORM_LINUX)
#   ifdef CLOCK_MONOTONIC_RAW
#       define MCR_TIMER_CLOCK_ID CLOCK_MONOTONIC_RAW
#   else
#       define MCR_TIMER_CLOCK_ID CLOCK_MONOTONIC
#   endif

struct Timer::Impl
{
    timespec start;
};

void Timer::_initFrequency()
{
    m_frequency = 1000000000ll; // nanoseconds
}

void Timer::_start()
{
    clock_gettime(MCR_TIMER_CLOCK_ID, &m_impl.start);
}

int64 Timer::_ticksSinceStart() const
{
    timespec ts;
    clock_gettime(MCR_TIMER_CLOCK_ID, &ts);

    return 1000000000ll * (ts.tv_sec - m_impl.start.tv_sec) + (ts.tv_nsec - m_impl.start.tv_nsec);
}

#   undef MCR_TIMER_CLOCK_ID
#endif


// ctr/dtr go here

Timer::Timer():
    m_impl(*new Impl)
{
    _initFrequency();
    start();
}

Timer::~Timer()
{
    delete &m_impl;
}

} // ns mcr
