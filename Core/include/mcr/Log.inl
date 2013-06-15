namespace mcr {

//////////////////////////////////////////////////////////////////////////
// Accessors & mutators

inline io::IWriter* Log::stream() const
{
    return m_stream;
}

inline void Log::setStream(io::IWriter* stream)
{
    m_stream = stream;
}


inline Log::Verbosity Log::verbosity() const
{
    return m_verbosity;
}

inline void Log::setVerbosity(Verbosity level)
{
    m_verbosity = level;
}


inline bool Log::isStdOutEnabled() const
{
    return m_stdout;
}

inline void Log::setStdOutEnabled(bool enabled)
{
    m_stdout = enabled;
}


//////////////////////////////////////////////////////////////////////////
// Logging

inline bool Log::error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    return !!vprint(Errors, fmt, args);
}

inline bool Log::warn(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    return !!vprint(Warnings, fmt, args);
}

inline bool Log::info(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    return !!vprint(Info, fmt, args);
}

inline bool Log::debug(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    return !!vprint(Debug, fmt, args);
}

inline std::size_t Log::print(Verbosity level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    return vprint(level, fmt, args);
}

} // ns mcr
