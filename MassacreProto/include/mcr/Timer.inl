namespace mcr {

inline void Timer::start()
{
    m_ticks = m_dticks = 0;
    m_mcsec = m_dmcsec = 0;
    m_msec  = m_dmsec  = 0;
    m_sec   = m_dsec   = 0;

    _start();
}

inline void Timer::stop()
{
    refresh();
}

inline void Timer::refresh()
{
    int64  curTicks = _ticksSinceStart();
    double curSec   = double(curTicks) / m_frequency;
    int64  curMsec  = int64(1000.0 * curSec);
    int64  curMcsec = int64(1000000.0 * curSec);

    // measure delta
    m_dticks = curTicks - m_ticks;
    m_dmcsec = curMcsec - m_mcsec;
    m_dmsec  = curMsec  - m_msec;
    m_dsec   = curSec   - m_sec;

    // remember the now
    m_ticks = curTicks;
    m_mcsec = curMcsec;
    m_msec  = curMsec;
    m_sec   = curSec;
}


inline int64 Timer::ticks() const
{
    return m_ticks;
}

inline int64 Timer::dticks() const
{
    return m_dticks;
}

inline int64 Timer::microseconds() const
{
    return m_mcsec;
}

inline int64 Timer::dmicroseconds() const
{
    return m_dmcsec;
}

inline int64 Timer::milliseconds() const
{
    return m_msec;
}

inline int64 Timer::dmilliseconds() const
{
    return m_dmsec;
}

inline double Timer::seconds() const
{
    return m_sec;
}

inline double Timer::dseconds() const
{
    return m_dsec;
}

} // ns mcr
