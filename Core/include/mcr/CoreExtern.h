#pragma once

#include <mcr/Platform.h>

#ifdef MCR_CORE_STATIC
#   define MCR_CORE_EXTERN 
#   define MCR_CORE_INTERN 
#else
#   ifdef MCR_CORE_EXPORTS
#       define MCR_CORE_EXTERN MCR_PLATFORM_EXPORT_SPEC
#   else
#       define MCR_CORE_EXTERN MCR_PLATFORM_IMPORT_SPEC
#   endif
#   define MCR_CORE_INTERN MCR_PLATFORM_INTERNAL_SPEC
#endif
