#pragma once

#include <mcr/MassacreTypes.h>

namespace mcr {

class Timer
{
public:
    MCR_EXTERN Timer();
    MCR_EXTERN ~Timer(); // not intended to be inhertied

    void    start();
    void    stop();
    void    refresh();

    int64   ticks() const;
    int64   dticks() const;

    int64   microseconds() const;
    int64   dmicroseconds() const;

    int64   milliseconds() const;
    int64   dmilliseconds() const;

    double  seconds() const;
    double  dseconds() const;

private:
    MCR_INTERN void  _init();
    MCR_INTERN void  _destroy();
    MCR_EXTERN void  _start();
    MCR_EXTERN int64 _ticksSinceStart() const;

    struct Impl;
    Impl& m_impl;

    int64 m_frequency;
    int64 m_ticks, m_mcsec, m_msec;
    int64 m_dticks, m_dmcsec, m_dmsec;
    double m_sec, m_dsec;
};

} // ns mcr

#include "Timer.inl"
