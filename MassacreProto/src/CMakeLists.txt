cmake_minimum_required(VERSION 2.6)

include_directories(${PROJECT_SOURCE_DIR}/MassacreProto/include)
include_directories(${PROJECT_SOURCE_DIR}/MassacreProto/src)
include_directories(${PROJECT_SOURCE_DIR}/MassacreProto/extras/SimpleMesh)
include_directories(${PROJECT_SOURCE_DIR}/MassacreShared/include)

set(GFXE_PATH gfx/experimental)
set(GFX_EXPERIMENTAL_SRC
    ${GFXE_PATH}/GeometryBuffer.cpp)

set(GFX_PATH gfx)
set(GFX_SRC
    ${GFX_PATH}/Context.cpp
    ${GFX_PATH}/GBuffer.cpp
    ${GFX_PATH}/GLState.cpp
    ${GFX_PATH}/GType.cpp
    ${GFX_PATH}/Image.cpp
    ${GFX_PATH}/ImageFormat.cpp
    ${GFX_PATH}/Material.cpp
    ${GFX_PATH}/MaterialManager.cpp
    ${GFX_PATH}/MaterialParameterBuffer.cpp
    ${GFX_PATH}/OldMeshes.cpp
    ${GFX_PATH}/Shader.cpp
    ${GFX_PATH}/ShaderPreprocessor.cpp
    ${GFX_PATH}/SimpleMeshExporter.cpp
    ${GFX_PATH}/SimpleMeshImporter.cpp
    ${GFX_PATH}/Texture.cpp
    ${GFX_PATH}/VertexArray.cpp
    ${GFX_PATH}/VertexFormat.cpp)

set(MASSACRE_SRC
    Camera.cpp
    Config.cpp
    Debug.cpp
    FileSystem.cpp
    Timer.cpp
    Universe.cpp)

link_libraries(
    GL
    ${GLEW_LIBRARY}
    ${GLFW_LIBRARIES}
    jpeg
    png
    yaml-cpp)

add_executable(massacre-demo
               ${MASSACRE_SRC}
               ${GFX_SRC}
               ${GFX_EXPERIMENTAL_SRC}
               main.cpp)

add_library(massacre SHARED
            ${MASSACRE_SRC}
            ${GFX_SRC}
            ${GFX_EXPERIMENTAL_SRC})

install(TARGETS massacre-demo
        DESTINATION bin)

install(TARGETS massacre
        DESTINATION lib)
