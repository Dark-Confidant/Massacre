#pragma once

#include "math/Math.h"
#include "Animation.h"

namespace mcr {

class AnimationController
{
public:
    AnimationController(): m_animation(), m_startFrame(0), m_endFrame(0), m_length(0), m_speed(1) {}
    ~AnimationController() {}

    Animation* animation() const
    {
        return m_animation;
    }

    void setAnimation(Animation* animation)
    {
        m_animation = animation;
    }

    void setInterval(float startFrame, float endFrame)
    {
        m_startFrame = startFrame;
        m_endFrame = endFrame;
        m_length = endFrame - startFrame;
    }

    float speed() const
    {
        return m_speed;
    }

    void setSpeed(float speed)
    {
        m_speed = speed;
    }

    void update(float time)
    {
        m_animation->setFrame(m_startFrame + fmod(m_speed * time, m_length));
    }

protected:
    Animation* m_animation;
    float m_startFrame, m_endFrame, m_length, m_speed;
};

} // ns mcr
