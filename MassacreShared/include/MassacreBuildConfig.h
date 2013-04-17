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
#   define MCR_INTERN 
#elif defined(__GNUC__) && (__GNUC__ >= 4)
#   define MCR_EXTERN __attribute__ ((visibility ("default")))
#   define MCR_PLUGIN_EXTERN MCR_EXTERN
#   define MCR_INTERN __attribute__ ((visibility ("hidden")))
#else
#   define MCR_EXTERN 
#   define MCR_PLUGIN_EXTERN 
#   define MCR_INTERN 
#endif
