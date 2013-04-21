#include "Universe.h"
#include "Camera.h"

#include <ctime>

using namespace mcr;

Camera::Camera():
    m_fov(65.f * math::deg2rad),
    m_zRange(1, 10),
    m_aspectRatio(4.f / 3.f)
{
    m_ubo = gfx::GBuffer::create(gfx::GBuffer::UniformBuffer);

    dumpMatrices();
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo->handle());
}

void Camera::dumpMatrices()
{
    struct
    {
        mat4 proj, view;
        float time;
    }
    m;

    m.proj = m_projection.first;  m.proj.transpose();
    m.view = m_view;              m.view.transpose();
    m.time = float(clock()) / CLOCKS_PER_SEC; // TODO: switch to Timer

    //m_ubo->init(nullptr, 0, gfx::GBuffer::DynamicDraw);
    m_ubo->init(&m, sizeof(m), gfx::GBuffer::DynamicDraw);
}
