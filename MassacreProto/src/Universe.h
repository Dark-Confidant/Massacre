#pragma once

#include <mcr/BuildConfig.h>

#include <cassert>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#if defined(MCR_PLATFORM_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <Windows.h>
#endif

#define GLEW_STATIC
#define GLEW_NO_GLU
#include <GL/glew.h>
