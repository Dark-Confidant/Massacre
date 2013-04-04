#pragma once

#include <stack>
#include "math/Matrix.h"
#include "gfx/GBuffer.h"

namespace mcr {

class Camera
{
public:
    Camera();

    const mat4& modelMatrix() const
    { return m_model; }

    void setModelMatrix(const mat4& m)
    {
        m_model = m;
        recalcModelViewMatrix();
    }

    void recalcModelViewMatrix()
    {
        m_mv.first = m_view * modelMatrix();
        m_mv.second = false;

        recalcModelViewProjMatrix();
    }

    const mat4& modelViewMatrix() const
    { return m_mv.first; }

    void recalcModelViewProjMatrix()
    {
        m_mvp.first = m_projection.first * m_mv.first;
        m_mvp.second = false;
    }

    const mat4& modelViewProjMatrix() const
    { return m_mvp.first; }


    const vec3& position() const
    { return m_position; }

    void setPosition(const vec3& pos)
    {
        m_position = pos;
        m_view.setTranslation(-pos * m_view);
        m_mv.second = true;
    }

    const vec3& rotation() const
    { return m_rotation; }

    void setRotation(const vec3& rot)
    {
        m_rotation = rot;
        m_view.setRotation(m_rotation);
        m_mv.second = true;
    }

    void lookAt(const vec3& target)
    {
        using namespace math;

        static const vec3 up(0, 1, 0);

        auto forward = normalize(target - m_position);
        auto strafe = normalize(cross(forward, up));

        m_view.vecAt<3>(0) = strafe;
        m_view.vecAt<3>(4) = normalize(cross(strafe, forward));
        m_view.vecAt<3>(8) = -forward;

        m_view.invertRotation();

        m_mv.second = true;
    }

    const mat4& viewMatrix() const
    { return m_view; }

    void setViewMatrix(const mat4& view, bool invertRotation = false)
    {
        m_view = view;

        if (invertRotation)
            m_view.invertRotation();

        m_mv.second = true;
    }

    float fov() const
    { return m_fov; }

    void setFov(float fov)
    {
        m_fov = fov;
        m_projection.second = true;
    }

    const vec2& zRange() const
    { return m_zRange; }

    void setZRange(const vec2& range)
    {
        m_zRange = range;
        m_projection.second = true;
    }

    float aspectRatio() const
    { return m_aspectRatio; }

    void setAspectRatio(float ratio)
    {
        m_aspectRatio = ratio;
        m_projection.second = true;
    }

    void setPerspective()
    {
        float ymax = m_zRange[0] * tan(.5f * m_fov);
        float xmax = ymax * m_aspectRatio;

        setFrustum(-xmax, xmax, -ymax, ymax);
    }

    void setFrustum(float left, float right, float bottom, float top)
    {
        float
            n = 2.f * m_zRange[0],
            w = right - left,
            h = top - bottom,
            d = m_zRange[1] - m_zRange[0];

        float* m = m_projection.first;

        memset(m, 0, sizeof(mat4));

        m[0] = n / w;
        m[5] = n / h;
        m[8] = (right + left) / w;
        m[9] = (top + bottom) / h;
        m[10] = (-m_zRange[1] - m_zRange[0]) / d;
        m[11] = -1.f;
        m[14] = (-n * m_zRange[1]) / d;

        m_projection.second = false;

        recalcModelViewMatrix();
    }

    void recalcProjMatrix() { setPerspective(); }

    const mat4& projMatrix() const { return m_projection.first; }

    void setProjMatrix(const mat4& proj)
    {
        m_projection.first = proj;
        m_mv.second = true;
    }

    void update()
    {
        if (m_projection.second)
            recalcProjMatrix();

        else if (m_mv.second)
            recalcModelViewMatrix();

        else if (m_mvp.second)
            recalcModelViewProjMatrix();
    }

    void dumpMatrices();

protected:
    std::pair<mat4, bool> m_projection, m_mv, m_mvp;
    mat4 m_model, m_view;
    vec3 m_position, m_rotation;
    float m_fov;
    vec2 m_zRange;
    float m_aspectRatio;
    rcptr<gfx::GBuffer> m_ubo;
};

} // ns mcr
