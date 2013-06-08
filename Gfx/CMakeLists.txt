cmake_minimum_required(VERSION 2.6)

find_package(Boost REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/Core/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/extras/SimpleMesh"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src")

link_libraries(
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    massacre-core)

file(GLOB_RECURSE Sources RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" src/*.cpp)

add_definitions(-DMCR_GFX_EXPORTS)
add_library(massacre-gfx SHARED ${Sources})
set_target_properties(massacre-gfx PROPERTIES DEBUG_POSTFIX d)

if(MSVC)
    set_target_properties(massacre-gfx PROPERTIES LINK_FLAGS_RELEASE /NODEFAULTLIB:libcmt)
    set_source_files_properties(${Sources} PROPERTIES COMPILE_FLAGS -YuUniverse.h)
    set_source_files_properties(src/Universe.cpp PROPERTIES COMPILE_FLAGS -YcUniverse.h)
else()
    set_source_files_properties(${Sources} PROPERTIES OBJECT_DEPENDS src/Universe.h.gch)
    set_source_files_properties(src/Universe.cpp PROPERTIES OBJECT_DEPENDS "")
    set_source_files_properties(src/Universe.cpp PROPERTIES OBJECT_OUTPUTS src/Universe.h.gch)
endif()

install(DIRECTORY include/ DESTINATION include)

if(WIN32)
    install(TARGETS massacre-gfx
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib)
else()
    install(TARGETS massacre-gfx
            DESTINATION lib)
endif()
