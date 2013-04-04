#pragma once

#include <map>
#include "math/Vector.h"
#include "Animation.h"
#include "Channel.h"

namespace mcr {

template <typename T>
class TypedAnimationCurve: public Animation
{
public:
    TypedAnimationCurve<T>() {}

    void setFrame(float frame)
    {
        m_channel << interpolate(frame);
    }

    T interpolate(float frame) const
    {
        auto iframe = int(frame + 1.f);

        auto it = m_keyFrames.lower_bound(iframe);
        if (it == m_keyFrames.end())
            return T();

        if (it == m_keyFrames.begin())
            return it->second;

        auto prev = std::prev(it);

        auto alpha = (frame - prev->first) / (it->first - prev->first);

        return prev->second + alpha * (it->second - prev->second);
    }

    void setKeyFrame(int frame, const T& value)
    {
        m_keyFrames[frame] = value;
    }

    void setKeyFrame(int frame, T&& value)
    {
        m_keyFrames[frame] = value;
    }

    void unsetKeyFrame(int frame)
    {
        m_keyFrames.erase(frame);
    }

    const OutputChannel<T>& channel() const
    {
        return m_channel;
    }

    void setChannel(const OutputChannel<T>& channel)
    {
        m_channel = channel;
    }

protected:
    std::map<int, T> m_keyFrames;
    OutputChannel<T> m_channel;
};

} // ns mcr
