#pragma once

#include <cassert>

#if defined(_WIN32) || defined(_WIN64)
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <Windows.h>
#   define MCR_PLATFORM_WINDOWS
#elif defined(__GNUC__)
#   define MCR_PLATFORM_LINUX
#endif

#define GLEW_STATIC
#include <GL/glew.h>
#if defined(MCR_PLATFORM_WINDOWS)
#   include <GL/wglew.h>
#elif defined(MCR_PLATFORM_LINUX)
#   include <GL/glxew.h>
#else
#   error Unknown platform
#endif
#undef GLEW_STATIC

#include <SDL/SDL.h>
