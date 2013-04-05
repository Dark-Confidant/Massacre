cmake_minimum_required(VERSION 2.6)

include_directories(${PROJECT_SOURCE_DIR}/MassacreProto/include)
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/MassacreProto/src)
include_directories(${PROJECT_SOURCE_DIR}/MassacreProto/extras/SimpleMesh)
include_directories(${PROJECT_SOURCE_DIR}/MassacreShared/include)

set(COMPONENTS_PATH components)
set(COMPONENTS_SRC 
  ${COMPONENTS_PATH}/Hierarchy.cpp 
  ${COMPONENTS_PATH}/Model.cpp 
  ${COMPONENTS_PATH}/Movement.cpp 
  ${COMPONENTS_PATH}/Render.cpp
  ${COMPONENTS_PATH}/Transform.cpp)

set(GFXR_PATH gfx/renderables)
set(GFX_RENDERABLES_SRC
  ${GFXR_PATH}/Mesh.cpp  
  ${GFXR_PATH}/Renderable.cpp 
  ${GFXR_PATH}/SpriteBatch.cpp)
  
set(GFX_PATH gfx)
set(GFX_SRC 
  ${GFX_PATH}/GLUtility.cpp 
  ${GFX_PATH}/Font.cpp 
  ${GFX_PATH}/Image.cpp
  ${GFX_PATH}/Material.cpp 
  ${GFX_PATH}/Mesh2.cpp 
  ${GFX_PATH}/Renderer.cpp
  ${GFX_PATH}/Shader.cpp 
  ${GFX_PATH}/Texture.cpp
  ${GFX_PATH}/VertexFormat.cpp 
  ${GFX_PATH}/Context.cpp
  ${GFX_PATH}/GBuffer.cpp
  ${GFX_PATH}/GPool.cpp
  ${GFX_PATH}/ImageFormat.cpp 
  ${GFX_PATH}/MaterialManager.cpp 
  ${GFX_PATH}/RenderState.cpp
  ${GFX_PATH}/ShaderProgram.cpp
  ${GFX_PATH}/VertexArray.cpp
  ${GFX_PATH}/VertexPool.cpp)  

set(MASSACRE_SRC
  Camera.cpp 
  ComRegistry.cpp 
  Debug.cpp 
  FileSystem.cpp 
  Object.cpp
  SmallObject.cpp 
  SuperGlobal.cpp 
  Timeline.cpp 
  Universe.cpp)
	
add_executable(massacre 
  ${GFX_SRC} 
  ${GFX_RENDERABLES_SRC} 
  ${COMPONENTS_SRC} 
  ${MASSACRE_SRC} 
  main.cpp)
