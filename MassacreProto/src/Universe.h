#pragma once

#include <mcr/MassacreBuildConfig.h>

#include <cassert>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#if defined(MCR_PLATFORM_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <Windows.h>
#endif

#define GLEW_STATIC
#include <GL/glew.h>
