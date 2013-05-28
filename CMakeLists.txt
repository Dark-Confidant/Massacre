cmake_minimum_required(VERSION 2.6)

project(Massacre)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-scripts/")

if (MSVC)
    set(CMAKE_CXX_FLAGS "-W3 -EHsc")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
else (MSVC)
    set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3 -fPIC")
endif (MSVC)

if (WIN32)
    set(CMAKE_INSTALL_PREFIX "$ENV{MASSACRE_ROOT}")
    if (NOT CMAKE_INSTALL_PREFIX)
        set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
    endif (NOT CMAKE_INSTALL_PREFIX)
    message("Install prefix: ${CMAKE_INSTALL_PREFIX}")
endif (WIN32)

add_subdirectory(Core)
add_subdirectory(Gfx)
add_subdirectory(Samples)
