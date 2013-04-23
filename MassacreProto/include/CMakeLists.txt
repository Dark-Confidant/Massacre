cmake_minimum_required(VERSION 2.6)

set(HEADERS
    mcr/Camera.h
    mcr/Config.h
    mcr/Debug.h
    mcr/ExternalSingleton.h
    mcr/FileSystem.h
    mcr/Io.h
    mcr/IPlugin.h
    mcr/Timer.h
    mcr/Timer.inl)
set(HEADERS_GFX
    mcr/gfx/Context.h
    mcr/gfx/Context.inl
    mcr/gfx/GBuffer.h
    mcr/gfx/GLUtility.h
    mcr/gfx/GType.h
    mcr/gfx/Image.h
    mcr/gfx/Image.inl
    mcr/gfx/ImageFormat.h
    mcr/gfx/ISprite.h
    mcr/gfx/Material.h
    mcr/gfx/MaterialManager.h
    mcr/gfx/RenderAtom.h
    mcr/gfx/RenderState.h
    mcr/gfx/RenderState.inl
    mcr/gfx/Shader.h
    mcr/gfx/Shader.inl
    mcr/gfx/ShaderProgram.h
    mcr/gfx/Texture.h
    mcr/gfx/VertexArray.h
    mcr/gfx/VertexFormat.h
    mcr/gfx/VertexFormat.inl)
set(HEADERS_GFXE
    mcr/gfx/experimental/IImageImporter.h
    mcr/gfx/experimental/IMeshExporter.h
    mcr/gfx/experimental/IMeshImporter.h
    mcr/gfx/experimental/Mesh.h
    mcr/gfx/experimental/MeshStorage.h)
set(HEADERS_GFXR
    mcr/gfx/renderables/Mesh.h
    mcr/gfx/renderables/Renderable.h
    mcr/gfx/renderables/SpriteBatch.h)

install(FILES ${HEADERS}
        DESTINATION include/mcr)
install(FILES ${HEADERS_GFX}
        DESTINATION include/mcr/gfx)
install(FILES ${HEADERS_GFXE}
        DESTINATION include/mcr/gfx/experimental)
install(FILES ${HEADERS_GFXR}
        DESTINATION include/mcr/gfx/renderables)
