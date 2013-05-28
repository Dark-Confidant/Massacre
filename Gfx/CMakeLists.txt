cmake_minimum_required(VERSION 2.6)

if (WIN32)
    set(Boost_USE_STATIC_LIBS ON)
else (WIN32)
    set(Boost_USE_STATIC_LIBS OFF)
endif (WIN32)

set(Boost_USE_MULTITHREADED ON)

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(YamlCpp REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${GLEW_INCLUDE_PATH}
    ${PNG_PNG_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${YAMLCPP_INCLUDE_DIR}
    "${PROJECT_SOURCE_DIR}/Core/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/extras/SimpleMesh"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src")

link_directories(
    ${Boost_LIBRARY_DIRS})

link_libraries(
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${ZLIB_LIBRARIES}
    ${PNG_LIBRARY}
    ${JPEG_LIBRARIES}
    ${YAMLCPP_LIBRARY}
    massacre-core)

file(GLOB_RECURSE Sources RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" src/*.cpp)

add_definitions(-DMCR_GFX_EXPORTS)
add_library(massacre-gfx SHARED ${Sources})
set_target_properties(massacre-gfx PROPERTIES DEBUG_POSTFIX d)

if (MSVC)
    set_source_files_properties(${Sources} PROPERTIES COMPILE_FLAGS -YuUniverse.h)
    set_source_files_properties(src/Universe.cpp PROPERTIES COMPILE_FLAGS -YcUniverse.h)
else (MSVC)
    set_source_files_properties(${Sources} PROPERTIES OBJECT_DEPENDS src/Universe.h.gch)
    set_source_files_properties(src/Universe.cpp PROPERTIES OBJECT_DEPENDS "")
    set_source_files_properties(src/Universe.cpp PROPERTIES OBJECT_OUTPUTS src/Universe.h.gch)
endif (MSVC)

install(DIRECTORY include/ DESTINATION include)
install(TARGETS massacre-gfx DESTINATION lib)
