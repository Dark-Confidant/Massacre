cmake_minimum_required(VERSION 2.6)

set(HEADERS
    Camera.h
    Config.h
    Debug.h
    ExternalSingleton.h
    FileSystem.h
    Io.h
    IPlugin.h
    Timer.h
    Timer.inl)
set(HEADERS_GFX
    gfx/Context.h
    gfx/Context.inl
    gfx/GBuffer.h
    gfx/GLUtility.h
    gfx/GType.h
    gfx/Image.h
    gfx/Image.inl
    gfx/ImageFormat.h
    gfx/ISprite.h
    gfx/Material.h
    gfx/MaterialManager.h
    gfx/RenderAtom.h
    gfx/RenderState.h
    gfx/Shader.h
    gfx/Shader.inl
    gfx/ShaderProgram.h
    gfx/Texture.h
    gfx/VertexArray.h
    gfx/VertexFormat.h
    gfx/VertexFormat.inl)
set(HEADERS_GFXE
    gfx/experimental/IImageImporter.h
    gfx/experimental/IMeshExporter.h
    gfx/experimental/IMeshImporter.h
    gfx/experimental/Mesh.h
    gfx/experimental/MeshStorage.h)
set(HEADERS_GFXR
    gfx/renderables/Mesh.h
    gfx/renderables/Renderable.h
    gfx/renderables/SpriteBatch.h)

install(FILES ${HEADERS}
        DESTINATION include/mcr)
install(FILES ${HEADERS_GFX}
        DESTINATION include/mcr/gfx)
install(FILES ${HEADERS_GFXE}
        DESTINATION include/mcr/gfx/experimental)
install(FILES ${HEADERS_GFXR}
        DESTINATION include/mcr/gfx/renderables)
