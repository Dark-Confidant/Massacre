cmake_minimum_required(VERSION 2.6)

find_package(Boost REQUIRED)
find_package(YamlCpp REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${YAMLCPP_INCLUDE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

link_libraries(
    ${YAMLCPP_LIBRARY})

file(GLOB_RECURSE Sources src/*.cpp)

add_definitions(-DMCR_CORE_EXPORTS)
add_library(massacre-core SHARED ${Sources})
set_target_properties(massacre-core PROPERTIES DEBUG_POSTFIX d)

install(DIRECTORY include/ DESTINATION include)
install(TARGETS massacre-core DESTINATION lib)
