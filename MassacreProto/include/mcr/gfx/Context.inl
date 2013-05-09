namespace mcr {
namespace gfx {

inline Context& Context::active()
{
    return *s_active;
}

inline const irect& Context::viewport() const
{
    return m_viewport;
}

inline const RenderState& Context::renderState() const
{
    return m_renderState;
}

inline Material* Context::activeMaterial() const
{
    return m_activeMaterial;
}

inline VertexArray* Context::activeVertexArray() const
{
    return m_activeVertexArray;
}

} // ns gfx
} // ns mcr
