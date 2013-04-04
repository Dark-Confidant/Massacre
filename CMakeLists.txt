cmake_minimum_required(VERSION 2.8)
project(Massacre)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-scripts/")

find_package(Boost REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(YamlCpp REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Freetype REQUIRED)