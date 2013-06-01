#pragma once

#include <cstdarg>
#include <sstream>
#include <mcr/CoreExtern.h>

namespace mcr {

MCR_CORE_EXTERN extern bool g_enableLoggingToFile;
MCR_CORE_EXTERN void vdebug(const char* fmt, va_list args);

inline void debug(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vdebug(fmt, args);
}

class DebugStream
{
public:
    enum FlushType { Flush };

    DebugStream& operator<<(FlushType)
    {
        debug("%s", m_stream.str().c_str());
        m_stream.str("");

        return *this;
    }

    template <typename T>
    DebugStream& operator<<(const T& thingy)
    {
        m_stream << thingy;
        return *this;
    }

private:
    std::stringstream m_stream;
};

} // ns mcr
