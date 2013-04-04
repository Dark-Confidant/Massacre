#pragma once

#include "math/Rect.h"
#include "RenderState.h"
#include <vector>

namespace mcr {
namespace gfx {

class ShaderProgram;
class Texture;
class Material;
class GBuffer;
class VertexArray;
struct Mesh2;

enum TexTarget
{
    Tex1D,
    Tex2D,
    Tex3D,
    TexCube,
    NumTexTargets
};

struct TextureUnit
{
    Texture* textures[NumTexTargets];
    uint refs;
};

enum BufferTarget
{
    VertexBuffer,
    IndexBuffer,
    UniformBuffer,
    NumBufferTargets
};

class Context
{
public:
    Context();
    ~Context(); // no inheritance

    static Context& active();

    bool activate();

    const irect& viewport() const;
    void setViewport(const irect& viewport);

    const RenderState& renderState() const;
    void setRenderState(const RenderState& rs);

    ShaderProgram* activeProgram() const;
    void setActiveProgram(ShaderProgram* program);

    uint activeTextureUnit() const;
    void setActiveTextureUnit(uint texUnit);

    Texture* activeTexture(TexTarget target, uint texUnit);
    void bindTexture(TexTarget target, Texture* tex);

    uint allocTextureUnit(uint* refs = nullptr);
    void freeTextureUnit(uint texUnit);

    Material* activeMaterial() const;
    void setActiveMaterial(Material* mtl);

    GBuffer* activeBuffer(BufferTarget target);
    GBuffer* activeBuffer(BufferTarget target, uint bufUnit);

    void bindBuffer(GBuffer* buffer);
    void bindBufferBase(uint bufUnit, GBuffer* buffer, uint offset);
    void bindBufferRange(uint bufUnit, GBuffer* buffer, uint offset, uint count);

    VertexArray* activeVertexArray() const;
    void setActiveVertexArray(VertexArray* va);

    void drawMesh(const Mesh2& mesh);

protected:
    static Context* s_active;

    irect          m_viewport;
    RenderState    m_renderState;
    uint           m_renderStateHash;
    ShaderProgram* m_activeProgram;

    uint                     m_activeTextureUnit;
    std::vector<TextureUnit> m_textureUnits;
    uint                     m_nextFreeTextureUnit;

    Material* m_activeMaterial;

    GBuffer*              m_buffers[NumBufferTargets];
    std::vector<GBuffer*> m_bufferUnits[NumBufferTargets];
    uint                  m_nextFreeBufferUnit;

    VertexArray* m_activeVertexArray;
};

} // ns gfx
} // ns mcr

#include "Context.inl"
