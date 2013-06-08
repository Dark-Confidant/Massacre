#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_DIRS
# GLEW_LIBRARIES
# 

find_path(GLEW_INCLUDE_DIRS GL/glew.h)

if(WIN32)
    find_library(GLEW_LIBRARY glew32s)
    find_library(GLEW_LIBRARY_DEBUG glew32sd)
else()
    find_library(GLEW_LIBRARY NAMES GLEW glew)
endif()

if(NOT GLEW_LIBRARY_DEBUG)
    set(GLEW_LIBRARY_DEBUG ${GLEW_LIBRARY})
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    GLEW DEFAULT_MSG
    GLEW_INCLUDE_DIRS GLEW_LIBRARY)

if(GLEW_FOUND)
    set(GLEW_LIBRARIES
        optimized ${GLEW_LIBRARY}
        debug ${GLEW_LIBRARY_DEBUG})
endif()

mark_as_advanced(GLEW_INCLUDE_DIRS GLEW_LIBRARIES GLEW_LIBRARY GLEW_LIBRARY_DEBUG)
