#pragma once

#include <map>
#include "Serializable.h"

namespace mcr {

struct DefaultTimelineIndex
{
    DefaultTimelineIndex(uint aobjId, ushort acomId):
        objId(aobjId), comId(acomId) {}

    Serializable* find() const
    {
        if (auto obj = Object::find(objId))
            return obj->com(comId);

        return nullptr;
    }

    uint objId;
    ushort comId;
};

template <class Index = DefaultTimelineIndex>
class Timeline
{
public:
    void set(uint64 time, const Index& idx, bb::ByteBuffer* data);
    void update(int64 dtime);

protected:
    virtual void interpolate(int64 dtime, void* dst) {}

private:
    struct Action
    {
        Index index;
        rcptr<bb::ByteBuffer> newData, oldData;
    };

    std::map<uint64, Action> m_flow;
    uint64 m_now;
};

template <class Index>
inline void Timeline<Index>::set(uint64 time, const Index& idx, bb::ByteBuffer* data)
{
    Action act = {idx (byte*) obj + offset, bytes, data};
    m_flow.emplace(std::make_pair(time, act));
}

template <class Index>
inline void Timeline<Index>::update(int64 dtime)
{
    m_now += dtime;
}

} // ns mcr
