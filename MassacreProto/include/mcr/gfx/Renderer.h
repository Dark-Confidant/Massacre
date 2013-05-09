#pragma once

#include <mcr/math/Rect.h>
#include <mcr/gfx/RenderState.h>

namespace mcr {
namespace gfx {

class Material;
class VertexArray;
struct Mesh;

class Renderer
{
public:
    Renderer();
    ~Renderer(); // inherit not

    const irect&        viewport() const;
    void                setViewport(const irect& vp);
                        
    const RenderState&  renderState() const;
    void                setRenderState(const RenderState& rs);

    Material*           activeMaterial() const;
    void                setActiveMaterial(Material* mtl);

    const VertexArray*  activeVertexArray() const;
    void                setActiveVertexArray(const VertexArray* va);

    void                drawMesh(const Mesh& mesh);

    void                clear();

protected:
    irect               m_viewport;
    RenderState         m_renderState;
    uint                m_renderStateHash;

    Material*           m_activeMaterial;

    const VertexArray*  m_activeVertexArray;
};

} // ns gfx
} // ns mcr

#include "Renderer.inl"
