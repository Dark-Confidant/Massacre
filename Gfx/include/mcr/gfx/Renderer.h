#pragma once

#include <mcr/math/Rect.h>
#include <mcr/gfx/mtl/Material.h>
#include <mcr/gfx/geom/Mesh.h>

namespace mcr {
namespace gfx {

class Renderer
{
public:
    MCR_GFX_EXTERN Renderer();
    MCR_GFX_EXTERN ~Renderer(); // inherit not

    MCR_GFX_EXTERN const irect& viewport() const;
    MCR_GFX_EXTERN void         setViewport(const irect& vp);
                        
    const mtl::RenderState&     renderState() const;
    MCR_GFX_EXTERN void         setRenderState(const mtl::RenderState& rs);

    mtl::Material*              activeMaterial() const;
    MCR_GFX_EXTERN void         setActiveMaterial(mtl::Material* material);

    MCR_GFX_EXTERN void         drawMesh(const geom::Mesh& mesh);

    MCR_GFX_EXTERN void         clear();

protected:
    irect               m_viewport;
    mtl::RenderState    m_renderState;
    uint                m_renderStateHash;

    mtl::Material*      m_activeMaterial;
};

} // ns gfx
} // ns mcr

#include "Renderer.inl"
