cmake_minimum_required(VERSION 2.6)
project(MASSACRE)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-scripts/")

find_package(Boost REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(YamlCpp REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${GLFW_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_PATH})

link_directories(${GLFW_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3")

add_subdirectory(MassacreProto)
add_subdirectory(MassacreShared)
