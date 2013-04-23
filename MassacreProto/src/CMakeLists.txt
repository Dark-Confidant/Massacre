cmake_minimum_required(VERSION 2.6)

include_directories(${PROJECT_SOURCE_DIR}/MassacreProto/include)
include_directories(${PROJECT_SOURCE_DIR}/MassacreProto/src)
include_directories(${PROJECT_SOURCE_DIR}/MassacreProto/extras/SimpleMesh)
include_directories(${PROJECT_SOURCE_DIR}/MassacreShared/include)

set(GFXE_PATH gfx/experimental)
set(GFX_EXPERIMENTAL_SRC
  ${GFXE_PATH}/SimpleMeshImporter.cpp
  ${GFXE_PATH}/SimpleMeshExporter.cpp)

set(GFXR_PATH gfx/renderables)
set(GFX_RENDERABLES_SRC
  ${GFXR_PATH}/Mesh.cpp
  ${GFXR_PATH}/SpriteBatch.cpp)

set(GFX_PATH gfx)
set(GFX_SRC
  ${GFX_PATH}/GLUtility.cpp
  ${GFX_PATH}/Image.cpp
  ${GFX_PATH}/Material.cpp
  ${GFX_PATH}/Shader.cpp
  ${GFX_PATH}/Texture.cpp
  ${GFX_PATH}/VertexFormat.cpp
  ${GFX_PATH}/Context.cpp
  ${GFX_PATH}/GBuffer.cpp
  ${GFX_PATH}/GType.cpp
  ${GFX_PATH}/ImageFormat.cpp
  ${GFX_PATH}/MaterialManager.cpp
  ${GFX_PATH}/ShaderProgram.cpp
  ${GFX_PATH}/VertexArray.cpp)

set(MASSACRE_SRC
  Camera.cpp
  Debug.cpp
  FileSystem.cpp
  Timer.cpp
  Config.cpp
  Universe.cpp)

link_libraries(
  ${SDL_LIBRARY}
  ${SDLIMAGE_LIBRARY}
  SDLmain
  GLEW
  GL
  jpeg
  png
  yaml-cpp)

add_executable(massacre-demo
  ${MASSACRE_SRC}
  ${GFX_SRC}
  ${GFX_RENDERABLES_SRC}
  ${GFX_EXPERIMENTAL_SRC}
  main.cpp)

add_library(massacre SHARED
  ${MASSACRE_SRC}
  ${GFX_SRC}
  ${GFX_RENDERABLES_SRC}
  ${GFX_EXPERIMENTAL_SRC})

install(TARGETS massacre-demo
        DESTINATION bin)

install(TARGETS massacre
        DESTINATION lib)
