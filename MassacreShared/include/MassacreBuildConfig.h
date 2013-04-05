#pragma once

#if defined(_WIN32) || defined(_WIN64)
#   define MCR_PLATFORM_WINDOWS
#elif defined(__GNUC__)
#   define MCR_PLATFORM_LINUX
#elif
#   error Unsupported platform
#endif
