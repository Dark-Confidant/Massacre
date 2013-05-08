#pragma once

#include <cstdarg>
#include <sstream>

namespace mcr {

extern bool g_enableLoggingToFile;

MCR_EXTERN void vdebug(const char* fmt, va_list args);
MCR_EXTERN void debug(const char* fmt, ...);

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

