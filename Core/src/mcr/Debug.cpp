#include <mcr/Debug.h>

#include <cstdarg>
#include <string>

#ifdef MCR_PLATFORM_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#endif

#ifdef _DEBUG
bool mcr::g_enableLoggingToFile = true;
#else
bool mcr::g_enableLoggingToFile = false;
#endif

void mcr::vdebug(const char* fmt, va_list args)
{
    struct LogHelper
    {
        static void log(const char* str)
        {
#ifdef MCR_PLATFORM_WINDOWS
            OutputDebugStringA(str);
#else
            fputs(str, stdout);
#endif
            if (g_enableLoggingToFile)
            {
                static FILE* fp = nullptr;
                if (!fp)
                {
#ifdef MCR_PLATFORM_WINDOWS
                    fp = _fsopen("debug.log", "w", _SH_DENYWR);
#else
                    fp = fopen("debug.log", "w");
#endif
                }
                if (fp)
                {
                    fputs(str, fp);
                    fflush(fp);
                }
            }
        }
    };

    std::string outputFmt = fmt;
    outputFmt += '\n';

    char buf[4096];
    if (vsnprintf(buf, sizeof(buf), outputFmt.c_str(), args) >= 0)
        LogHelper::log(buf);
    else
    {
        std::string largeBuf(1024 * 1024, '\0');
        while (vsnprintf(&largeBuf[0], largeBuf.size(), outputFmt.c_str(), args) < 0)
            largeBuf.resize(2 * largeBuf.size());

        LogHelper::log(largeBuf.c_str());
    }
}
