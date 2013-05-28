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
#   define MCR_PLATFORM_IMPORT_SPEC   __declspec(dllimport)
#   define MCR_PLATFORM_EXPORT_SPEC   __declspec(dllexport)
#   define MCR_PLATFORM_INTERNAL_SPEC 
#elif defined(__GNUC__) && (__GNUC__ >= 4)
#   define MCR_PLATFORM_IMPORT_SPEC   __attribute__ ((visibility ("default")))
#   define MCR_PLATFORM_EXPORT_SPEC   __attribute__ ((visibility ("default")))
#   define MCR_PLATFORM_INTERNAL_SPEC __attribute__ ((visibility ("hidden")))
#else
#   define MCR_PLATFORM_IMPORT_SPEC 
#   define MCR_PLATFORM_EXPORT_SPEC 
#   define MCR_PLATFORM_INTERNAL_SPEC 
#endif
