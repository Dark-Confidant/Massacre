cmake_minimum_required(VERSION 2.6)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE Sources src/*.cpp)

add_definitions(-DMCR_CORE_EXPORTS)
add_library(massacre-core SHARED ${Sources})
set_target_properties(massacre-core PROPERTIES DEBUG_POSTFIX d)

install(DIRECTORY include/ DESTINATION include)

if(WIN32)
    install(TARGETS massacre-core
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib)
else()
    install(TARGETS massacre-core
            DESTINATION lib)
endif()
