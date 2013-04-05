#include "Universe.h"
#include "Debug.h"

#include <string>
#include <cstdarg>

#ifdef _DEBUG
bool mcr::g_enableLoggingToFile = true;
#else
bool mcr::g_enableLoggingToFile = false;
#endif

void mcr::vdebug(const char* fmt, va_list args)
{
    char buf[4096];
    vsnprintf(buf, 4096, (std::string(fmt) + '\n').c_str(), args);

#ifdef MCR_PLATFORM_WINDOWS
    OutputDebugStringA(buf);
#endif

    if (g_enableLoggingToFile)
    {
        static FILE* fp = nullptr;

        if (!fp)
#ifdef MCR_PLATFORM_WINDOWS
            fp = _fsopen("debug.log", "w", _SH_DENYWR);
#else
            fp = fopen("debug.log", "w");
#endif

        if (fp)
        {
            fputs(buf, fp);
            fflush(fp);
        }
    }
}

void mcr::debug(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vdebug(fmt, args);
}
