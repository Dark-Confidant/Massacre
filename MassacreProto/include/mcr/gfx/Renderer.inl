namespace mcr {
namespace gfx {

inline const RenderState& Renderer::renderState() const
{
    return m_renderState;
}

inline Material* Renderer::activeMaterial() const
{
    return m_activeMaterial;
}

inline const VertexArray* Renderer::activeVertexArray() const
{
    return m_activeVertexArray;
}

} // ns gfx
} // ns mcr
