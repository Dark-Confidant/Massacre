#pragma once

#include <mcr/math/Matrix.h>
#include <mcr/gfx/MaterialParameterBuffer.h>

namespace mcr {
namespace gfx {

class Camera
{
public:
    MCR_EXTERN Camera();
    MCR_EXTERN ~Camera();

    const mat4&     modelMatrix() const;
    void            setModelMatrix(const mat4& m);

    const mat4&     viewMatrix() const;
    void            setViewMatrix(const mat4& view, bool invertRotation = false);

    const mat4&     projMatrix() const;
    void            setProjMatrix(const mat4& proj);

    const mat4&     modelViewMatrix() const;
    const mat4&     modelViewProjMatrix() const;

    const vec3&     position() const;
    void            setPosition(const vec3& pos);

    const vec3&     rotation() const;
    void            setRotation(const vec3& rot);

    MCR_EXTERN void lookAt(const vec3& target);

    float           fov() const;
    void            setFov(float fov);

    const vec2&     zRange() const;
    void            setZRange(const vec2& range);

    float           aspectRatio() const;
    void            setAspectRatio(float ratio);

    void            setPerspective();
    MCR_EXTERN void setFrustum(float left, float right, float bottom, float top);

    void            update();

    void            recalcProjMatrix();
    void            recalcModelViewMatrix();
    void            recalcModelViewProjMatrix();

    void            dumpMatrices();

    MaterialParameterBuffer* parameterBuffer() const;

protected:
    std::pair<mat4, bool>   m_projection, m_mv, m_mvp;
    mat4                    m_model, m_view;
    vec3                    m_position, m_rotation;
    float                   m_fov, m_aspectRatio;
    vec2                    m_zRange;

    rcptr<MaterialParameterBuffer> m_parameterBuffer;
};

} // ns gfx
} // ns mcr

#include "Camera.inl"
