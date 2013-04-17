#pragma once

#if defined(_WIN32) || defined(_WIN64)
#   define MCR_PLATFORM_WINDOWS
#elif defined(__GNUC__)
#   define MCR_PLATFORM_LINUX
#else
#   error Unsupported platform
#endif

#if defined(_MSC_VER)
#   if defined(MCR_DLL)
#       define MCR_EXTERN __declspec(dllexport)
#   else
#       define MCR_EXTERN __declspec(dllimport) 
#   endif
#   define MCR_PLUGIN_EXTERN __declspec(dllexport)
#elif defined(__GNUC__)
#   define MCR_EXTERN extern
#   define MCR_PLUGIN_EXTERN extern
#else
#   define MCR_EXTERN 
#   define MCR_PLUGIN_EXTERN 
#endif
