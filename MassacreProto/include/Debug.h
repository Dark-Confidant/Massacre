#pragma once

#include <cstdarg>

namespace mcr {

extern bool g_enableLoggingToFile;

void vdebug(const char* fmt, va_list args);
void debug(const char* fmt, ...);

}
