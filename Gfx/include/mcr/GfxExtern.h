#pragma once

#include <mcr/Platform.h>

#ifdef MCR_GFX_STATIC
#   define MCR_GFX_EXTERN 
#   define MCR_GFX_INTERN 
#else
#   ifdef MCR_GFX_EXPORTS
#       define MCR_GFX_EXTERN MCR_PLATFORM_EXPORT_SPEC
#   else
#       define MCR_GFX_EXTERN MCR_PLATFORM_IMPORT_SPEC
#   endif
#   define MCR_GFX_INTERN MCR_PLATFORM_INTERNAL_SPEC
#endif

#define MCR_GFX_PLUGIN_EXTERN extern "C" MCR_PLATFORM_EXPORT_SPEC
