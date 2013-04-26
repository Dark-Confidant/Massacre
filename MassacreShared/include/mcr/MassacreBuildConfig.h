#pragma once

#if defined(_WIN32) || defined(_WIN64)
#   define MCR_PLATFORM_WINDOWS
#elif defined(__MACH__)
#   define MCR_PLATFORM_MAC
#elif defined(__GNUC__)
#   define MCR_PLATFORM_LINUX
#else
#   error Unsupported platform
#endif

#if defined(_MSC_VER)
#   if defined(MCR_STATIC)
#       define MCR_EXTERN 
#       define MCR_PLUGIN_EXTERN 
#   else
#       if defined(MCR_DLL)
#           define MCR_EXTERN __declspec(dllexport)
#       else
#           define MCR_EXTERN __declspec(dllimport) 
#       endif
#       define MCR_PLUGIN_EXTERN extern "C" __declspec(dllexport)
#   endif
#   define MCR_INTERN 
#elif defined(__GNUC__) && (__GNUC__ >= 4)
#   define MCR_EXTERN __attribute__ ((visibility ("default")))
#   define MCR_PLUGIN_EXTERN extern "C" MCR_EXTERN 
#   define MCR_INTERN __attribute__ ((visibility ("hidden")))
#else
#   define MCR_EXTERN 
#   define MCR_PLUGIN_EXTERN extern "C"
#   define MCR_INTERN 
#endif