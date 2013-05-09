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
    mcr/gfx/GBuffer.h
    mcr/gfx/GBuffer.inl
    mcr/gfx/GType.h
    mcr/gfx/IImageImporter.h
    mcr/gfx/Image.h
    mcr/gfx/Image.inl
    mcr/gfx/ImageFormat.h
    mcr/gfx/IMeshExporter.h
    mcr/gfx/IMeshImporter.h
    mcr/gfx/IShaderPreprocessor.h
    mcr/gfx/Material.h
    mcr/gfx/Material.inl
    mcr/gfx/MaterialManager.h
    mcr/gfx/MaterialManager.inl
    mcr/gfx/MaterialParameter.h
    mcr/gfx/MaterialParameter.inl
    mcr/gfx/MaterialParameterBuffer.h
    mcr/gfx/MaterialParameterBuffer.inl
    mcr/gfx/Mesh.h
    mcr/gfx/OldMeshes.h
    mcr/gfx/Renderer.h
    mcr/gfx/Renderer.inl
    mcr/gfx/RenderState.h
    mcr/gfx/RenderState.inl
    mcr/gfx/Shader.h
    mcr/gfx/Shader.inl
    mcr/gfx/Texture.h
    mcr/gfx/VertexArray.h
    mcr/gfx/VertexFormat.h
    mcr/gfx/VertexFormat.inl)
set(HEADERS_GFXE
    mcr/gfx/experimental/GeometryBuffer.h)

install(FILES ${HEADERS}
        DESTINATION include/mcr)
install(FILES ${HEADERS_GFX}
        DESTINATION include/mcr/gfx)
install(FILES ${HEADERS_GFXE}
        DESTINATION include/mcr/gfx/experimental)
