#pragma once

#include <vector>
#include <mcr/math/Rect.h>
#include <mcr/gfx/RenderState.h>
#include <mcr/gfx/GBuffer.h>

namespace mcr {
namespace gfx {

class Texture;
class Material;
class VertexArray;
struct Mesh;

class Context
{
public:
    Context();
    ~Context(); // inherit not

    static Context&     active();
    bool                activate();

    const irect&        viewport() const;
    void                setViewport(const irect& viewport);
                        
    const RenderState&  renderState() const;
    void                setRenderState(const RenderState& rs);
                        
    uint                activeTextureUnit() const;
    void                setActiveTextureUnit(uint texUnit);
                        
    Texture*            activeTexture(uint texUnit);
    void                bindTexture(Texture* tex);
                        
    Material*           activeMaterial() const;
    void                setActiveMaterial(Material* mtl);
                        
    GBuffer*            activeBuffer(GBuffer::Type type);
    GBuffer*            activeBuffer(GBuffer::Type type, uint bufUnit);

    void                bindBuffer(GBuffer* buffer);

    VertexArray*        activeVertexArray() const;
    void                setActiveVertexArray(VertexArray* va);

    void                clear();

    void                drawMesh(const Mesh& mesh);

protected:
    static Context* s_active;

    irect          m_viewport;
    RenderState    m_renderState;
    uint           m_renderStateHash;

    uint                  m_activeTextureUnit;
    std::vector<Texture*> m_textureUnits;
    uint                  m_nextFreeTextureUnit;

    Material* m_activeMaterial;

    GBuffer*     m_buffers[GBuffer::NumTypes];
    VertexArray* m_activeVertexArray;
};

} // ns gfx
} // ns mcr

#include "Context.inl"
