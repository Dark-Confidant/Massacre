cmake_minimum_required(VERSION 2.6)

project(Massacre)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-scripts/")

set(Compiler ${CMAKE_CXX_COMPILER_ID})

if(MSVC)
    set(CMAKE_CXX_FLAGS "-W3 -EHsc")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
elseif(Compiler STREQUAL Clang)
    set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
else() # default to gcc-like flags
    set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3")
endif()

if(WIN32)
    set(CMAKE_INSTALL_PREFIX "$ENV{MASSACRE_ROOT}")
    if(NOT CMAKE_INSTALL_PREFIX)
        set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
    endif()
    message("Install prefix: ${CMAKE_INSTALL_PREFIX}")
endif()

add_subdirectory(Core)
add_subdirectory(Gfx)
add_subdirectory(Samples)
