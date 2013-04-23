#include "Universe.h"
#include <mcr/Timer.h>

#ifdef MCR_PLATFORM_LINUX
#   include <ctime>
#endif

namespace mcr {
#if defined(MCR_PLATFORM_WINDOWS)

struct Timer::Impl
{
    int64 start;
};

void Timer::implInitFrequency()
{
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
}

void Timer::implStart()
{
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_impl.start));
}

int64 Timer::implTicksSinceStart() const
{
    int64 result;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&result));

    return result - m_impl.start;
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

void Timer::implInitFrequency()
{
    m_frequency = 1000000000ll; // nanoseconds
}

void Timer::implStart()
{
    clock_gettime(MCR_TIMER_CLOCK_ID, &m_impl.start);
}

int64 Timer::implTicksSinceStart() const
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
    implInitFrequency();
    start();
}

Timer::~Timer()
{
    delete &m_impl;
}

} // ns mcr
