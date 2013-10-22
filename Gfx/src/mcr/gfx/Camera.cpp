#include "Universe.h"
#include <mcr/gfx/Camera.h>

#include <ctime>

namespace mcr {
namespace gfx {

Camera::Camera():
    m_fov(65.f * math::deg2rad),
    m_aspectRatio(4.f / 3.f),
    m_zRange(1, 10)
{
    m_paramBuffer = mtl::ParamBuffer::create(
        "Camera", mtl::ParamLayout()
            .addMat4("Projection")
            .addMat4("View")
            .addVec3("Position")
            .addVec3("Direction")
            .addVec2("ZRange"));

    dumpMatrices();
}

Camera::~Camera() {}


void Camera::dumpMatrices()
{
    m_paramBuffer->setParam(0, math::transpose(m_projection.first));
    m_paramBuffer->setParam(1, math::transpose(m_view));
    m_paramBuffer->setParam(2, m_position);
    m_paramBuffer->setParam(3, vec3(0, 0, 1) * m_view);
    m_paramBuffer->setParam(4, m_zRange);
}


void Camera::lookAt(const vec3& target)
{
    using namespace math;

    static const vec3 up(0, 1, 0);

    auto forward = normalize(target - m_position);
    auto strafe  = normalize(cross(forward, up));

    m_view.vecAt<3>(0) = strafe;
    m_view.vecAt<3>(4) = normalize(cross(strafe, forward));
    m_view.vecAt<3>(8) = -forward;

    m_view.invertRotation();

    m_mv.second = true;
}

void Camera::setFrustum(float left, float right, float bottom, float top)
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

} // ns gfx
} // ns mcr
