#include <mcr/Log.h>

#ifdef MCR_PLATFORM_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <Windows.h>
#endif
#include <cstdio>

namespace mcr {

std::size_t internalLogOutput(const char* str, io::IWriter* stream, bool stdoutToo)
{
    // TODO: handle line breaks

#ifdef MCR_PLATFORM_WINDOWS
    OutputDebugStringA(str);
#endif

    int bytes = 0;

    if (stream)
        bytes = (int) stream->write(str, std::strlen(str));

    if (stdoutToo)
        bytes = std::max(bytes, fputs(str, stdout));

    return (std::size_t) bytes;
}


std::size_t Log::vprint(Verbosity level, const char* fmt, std::va_list args)
{
    if (m_verbosity < level)
        return 0;

    static const std::string s_levelLiterals[] =
    {
        "",
        "[ERROR] ",
        "[WARNING] ",
        "[INFO] ",
        "[DEBUG] "
    };

    std::string outputFmt = s_levelLiterals[level];
    outputFmt += fmt;

#ifdef MCR_PLATFORM_WINDOWS
    outputFmt += "\r\n";
#else
    outputFmt += '\n';
#endif

    char buf[4096];
    if (vsnprintf(buf, sizeof(buf), outputFmt.c_str(), args) >= 0)
        return internalLogOutput(buf, m_stream, m_stdout);

    std::string largeBuf(1024 * 1024, '\0');
    while (vsnprintf(&largeBuf[0], largeBuf.size(), outputFmt.c_str(), args) < 0)
        largeBuf.resize(2 * largeBuf.size());

    return internalLogOutput(largeBuf.c_str(), m_stream, m_stdout);
}


Log* detail::LogSingleton::operator->() const
{
    static Log s_log;
    return &s_log;
}

} // ns mcr
