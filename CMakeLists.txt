cmake_minimum_required(VERSION 2.6)
project(MASSACRE)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-scripts/")

find_package(Boost REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(YamlCpp REQUIRED)
find_package(ZLIB REQUIRED)
find_package(FreeType REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -fpermissive")

add_subdirectory(MassacreProto)
