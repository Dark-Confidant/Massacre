#include "Universe.h"
#include <mcr/Camera.h>

#include <ctime>

namespace mcr {

Camera::Camera():
    m_fov(65.f * math::deg2rad),
    m_zRange(1, 10),
    m_aspectRatio(4.f / 3.f),
    m_parameterBuffer(
        gfx::MaterialParameterBuffer::create("Camera",
            gfx::MaterialParameterBufferLayout()
                (gfx::MaterialParameterType::Mat4, "Projection")
                (gfx::MaterialParameterType::Mat4, "View")
                (gfx::MaterialParameterType::Vec3, "Position")
                (gfx::MaterialParameterType::Vec3, "Direction")))
{
    dumpMatrices();
}

void Camera::dumpMatrices()
{
    m_parameterBuffer->parameter(0) = math::transpose(m_projection.first);
    m_parameterBuffer->parameter(1) = math::transpose(m_view);
    m_parameterBuffer->parameter(2) = m_position;
    m_parameterBuffer->parameter(3) = vec3(0, 0, 1) * m_view;
    m_parameterBuffer->sync();
}

} // ns mcr
