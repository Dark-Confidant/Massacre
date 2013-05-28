namespace mcr {
namespace gfx {

inline const mat4& Camera::modelMatrix() const
{
    return m_model;
}

inline void Camera::setModelMatrix(const mat4& m)
{
    m_model = m;
    recalcModelViewMatrix();
}

inline const mat4& Camera::viewMatrix() const
{
    return m_view;
}

inline void Camera::setViewMatrix(const mat4& view, bool invertRotation /*= false*/)
{
    m_view = view;

    if (invertRotation)
        m_view.invertRotation();

    m_mv.second = true;
}

inline const mat4& Camera::projMatrix() const
{
    return m_projection.first;
}

inline void Camera::setProjMatrix(const mat4& proj)
{
    m_projection.first = proj;
    m_mv.second = true;
}

inline const mat4& Camera::modelViewMatrix() const
{
    return m_mv.first;
}

inline const mat4& Camera::modelViewProjMatrix() const
{
    return m_mvp.first;
}

inline const vec3& Camera::position() const
{
    return m_position;
}

inline void Camera::setPosition(const vec3& pos)
{
    m_position = pos;
    m_view.setTranslation(-pos * m_view);
    m_mv.second = true;
}

inline const vec3& Camera::rotation() const
{
    return m_rotation;
}

inline void Camera::setRotation(const vec3& rot)
{
    m_rotation = rot;
    m_view.setRotation(m_rotation);
    m_mv.second = true;
}

inline float Camera::fov() const
{
    return m_fov;
}

inline void Camera::setFov(float fov)
{
    m_fov = fov;
    m_projection.second = true;
}

inline const vec2& Camera::zRange() const
{
    return m_zRange;
}

inline void Camera::setZRange(const vec2& range)
{
    m_zRange = range;
    m_projection.second = true;
}

inline float Camera::aspectRatio() const
{
    return m_aspectRatio;
}

inline void Camera::setAspectRatio(float ratio)
{
    m_aspectRatio = ratio;
    m_projection.second = true;
}

inline void Camera::setPerspective()
{
    float ymax = m_zRange[0] * tan(.5f * m_fov);
    float xmax = ymax * m_aspectRatio;

    setFrustum(-xmax, xmax, -ymax, ymax);
}

inline void Camera::update()
{
    if (m_projection.second)
        recalcProjMatrix();

    else if (m_mv.second)
        recalcModelViewMatrix();

    else if (m_mvp.second)
        recalcModelViewProjMatrix();
}

inline void Camera::recalcProjMatrix()
{
    setPerspective();
}

inline void Camera::recalcModelViewMatrix()
{
    m_mv.first = m_view * modelMatrix();
    m_mv.second = false;

    recalcModelViewProjMatrix();
}

inline void Camera::recalcModelViewProjMatrix()
{
    m_mvp.first = m_projection.first * m_mv.first;
    m_mvp.second = false;
}

inline mtl::ParamBuffer* Camera::paramBuffer() const
{
    return m_paramBuffer;
}

} // ns gfx
} // ns mcr
