#include "Universe.h"
#include <mcr/Timer.h>

#if defined(MCR_PLATFORM_MAC)
#   include <mach/mach.h>
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

void Timer::_init()
{
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
}

void Timer::_destroy() {}

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
    clock_serv_t service;
    mach_timespec_t start;
};

void Timer::_init()
{
    m_frequency = 1000000000ll; // nanoseconds
    host_get_clock_service(mach_host_self(), REALTIME_CLOCK, &m_impl.service);
}

void Timer::_destroy()
{
    mach_port_deallocate(mach_task_self(), m_impl.service);
}

void Timer::_start()
{
    clock_get_time(m_impl.service, &m_impl.start);
}

void Timer::_ticksSinceStart()
{
    mach_timespec_t now;
    clock_get_time(m_impl.service, &now);

    return 1000000000ll * (now.tv_sec - m_impl.start.tv_sec) + (now.tv_nsec - m_impl.start.tv_nsec);
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

void Timer::_init()
{
    m_frequency = 1000000000ll; // nanoseconds
}

void Timer::_destroy() {}

void Timer::_start()
{
    clock_gettime(MCR_TIMER_CLOCK_ID, &m_impl.start);
}

int64 Timer::_ticksSinceStart() const
{
    timespec now;
    clock_gettime(MCR_TIMER_CLOCK_ID, &now);

    return 1000000000ll * (now.tv_sec - m_impl.start.tv_sec) + (now.tv_nsec - m_impl.start.tv_nsec);
}

#   undef MCR_TIMER_CLOCK_ID
#endif


// ctr/dtr go here

Timer::Timer():
    m_impl(*new Impl)
{
    _init();
    start();
}

Timer::~Timer()
{
    _destroy();
    delete &m_impl;
}

} // ns mcr
