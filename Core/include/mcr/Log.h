#pragma once

#include <cstddef>
#include <cstdarg>
#include <mcr/io/IWriter.h>

namespace mcr {

class Log
{
public:
    enum Verbosity
    {
        Quiet,
        Errors,
        Warnings,
        Info,
        Debug
    };

    Log(): m_verbosity(Info), m_stdout(true) {}
    ~Log() {}

    // Stream to be spammed
    io::IWriter*                    stream() const;
    void                            setStream(io::IWriter* stream);

    // Output verbosity, i.e. logging level
    Verbosity                       verbosity() const;
    void                            setVerbosity(Verbosity level);

    // Whether to log to stdout (along with the stream, if set)
    bool                            isStdOutEnabled() const;
    void                            setStdOutEnabled(bool enabled);

    // Logging, naturally
    bool                            error(const char* fmt, ...);
    bool                            warn(const char* fmt, ...);
    bool                            info(const char* fmt, ...);
    bool                            debug(const char* fmt, ...);
    std::size_t                     print(Verbosity level, const char* fmt, ...);
    MCR_CORE_EXTERN std::size_t     vprint(Verbosity level, const char* fmt, std::va_list args);

private:
    rcptr<io::IWriter>  m_stream;
    Verbosity           m_verbosity;
    bool                m_stdout;
};


namespace detail {
struct LogSingleton
{
    MCR_CORE_EXTERN Log* operator->() const;
};
} // ns detail

static const detail::LogSingleton g_log;

} // ns mcr

#include "Log.inl"
