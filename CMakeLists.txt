cmake_minimum_required(VERSION 2.6)

project(Massacre)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-scripts/")

if(MSVC)
    set(CMAKE_CXX_FLAGS "-W3 -EHsc")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
else() # default to gcc-like flags
    set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3")
endif()

if(WIN32)
    set(MASSACRE_ROOT $ENV{MASSACRE_ROOT})
    if(NOT MASSACRE_ROOT)
        set(MASSACRE_ROOT ${PROJECT_SOURCE_DIR})
    endif()
    
    set(CMAKE_INSTALL_PREFIX ${MASSACRE_ROOT})
    message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
    
    file(GLOB _extras ${MASSACRE_ROOT}/extras/*)
    foreach(_extra ${_extras})
        if(IS_DIRECTORY ${_extra})
            list(APPEND CMAKE_PREFIX_PATH ${_extra})
        endif()
    endforeach()
endif()

add_subdirectory(Core)
add_subdirectory(Gfx)
add_subdirectory(Samples)
