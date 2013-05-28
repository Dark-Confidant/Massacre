namespace mcr {
namespace gfx {

inline const mtl::RenderState& Renderer::renderState() const
{
    return m_renderState;
}

inline mtl::Material* Renderer::activeMaterial() const
{
    return m_activeMaterial;
}

} // ns gfx
} // ns mcr
