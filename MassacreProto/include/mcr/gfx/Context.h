#pragma once

#include <vector>
#include <mcr/math/Rect.h>
#include <mcr/gfx/RenderState.h>

namespace mcr {
namespace gfx {

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

    Material*           activeMaterial() const;
    void                setActiveMaterial(Material* mtl);

    VertexArray*        activeVertexArray() const;
    void                setActiveVertexArray(VertexArray* va);

    void                drawMesh(const Mesh& mesh);

    void                clear();

protected:
    static Context* s_active;

    irect           m_viewport;
    RenderState     m_renderState;
    uint            m_renderStateHash;

    Material*       m_activeMaterial;

    VertexArray*    m_activeVertexArray;
};

} // ns gfx
} // ns mcr

#include "Context.inl"
