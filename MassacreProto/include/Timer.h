#pragma once

#include <MassacreTypes.h>

namespace mcr {

class Timer
{
public:
    Timer();
    ~Timer(); // not intended to be inhertied

    void start();
    void stop();
    void refresh();

    int64 ticks() const;
    int64 dticks() const;

    int64 microseconds() const;
    int64 dmicroseconds() const;

    int64 milliseconds() const;
    int64 dmilliseconds() const;

    double seconds() const;
    double dseconds() const;

private:
    void initFrequency();
    int64 queryTicks() const;

    int64 m_frequency;
    int64 m_startTick;
    int64 m_ticks, m_mcsec, m_msec;
    int64 m_dticks, m_dmcsec, m_dmsec;
    double m_sec, m_dsec;
};

} // ns mcr

#include "Timer.inl"
