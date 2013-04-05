#pragma once

#include "MassacreBuildConfig.h"

#if defined(MCR_PLATFORM_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <Windows.h>
#endif

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL/SDL.h>

#include <cassert>
#include <boost/foreach.hpp>
