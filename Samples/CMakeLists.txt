cmake_minimum_required(VERSION 2.6)

find_package(GLFW)

if(NOT GLFW_FOUND)
    message(STATUS "No GLFW found -- skipping samples")
else()
    include_directories(
        ${GLFW_INCLUDE_DIRS}
        "${PROJECT_SOURCE_DIR}/Core/include"
        "${PROJECT_SOURCE_DIR}/Gfx/include")

    link_libraries(
        ${GLFW_LIBRARIES}
        massacre-core
        massacre-gfx)

    add_executable(massacre-demo src/Demo.cpp)
    
    if(MSVC)
        set_target_properties(
            massacre-demo
            PROPERTIES LINK_FLAGS_DEBUG /NODEFAULTLIB:msvcrt
                       DEBUG_POSTFIX d)
    endif()

    install(TARGETS massacre-demo DESTINATION bin)
endif()
