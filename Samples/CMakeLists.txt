cmake_minimum_required(VERSION 2.6)

find_package(GLFW REQUIRED)

include_directories(
    ${GLFW_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/Core/include"
    "${PROJECT_SOURCE_DIR}/Gfx/include")

link_libraries(
    ${GLFW_LIBRARIES}
    massacre-core
    massacre-gfx)

add_executable(massacre-demo src/demo.cpp)

install(TARGETS massacre-demo DESTINATION bin)
