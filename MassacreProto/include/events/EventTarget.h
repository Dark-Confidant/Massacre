#pragma once

#include <unordered_map>
#include <unordered_set>
#include "EventArray.h"
#include "EventListener.h"

namespace mcr {

class EventTarget
{
public:
    virtual ~EventTarget() {}

    void addListener(EventId event, EventListener* listener);
    void addListener(const EventArray& events, EventListener* listener);

    void removeListener(EventId event, EventListener* listener);
    void removeListener(const EventArray& events, EventListener* listener);

    void notify(EventId event, const EventParams& params = EventParams());

protected:
    virtual void onAddListener(EventId, EventListener*) {}
    virtual void onRemoveListener(EventId, EventListener*) {}

    std::unordered_map<EventId, std::unordered_set<EventListener*>> m_listeners;
};


inline void EventTarget::addListener(EventId event, EventListener* listener)
{
    onAddListener(event, listener);
    m_listeners[event].insert(listener);
}

inline void EventTarget::addListener(const EventArray& events, EventListener* listener)
{
    for (int i = 0; i < events.size(); ++i)
        addListener(events[i], listener);
}

inline void EventTarget::removeListener(EventId event, EventListener* listener)
{
    m_listeners[event].erase(listener);
    onRemoveListener(event, listener);
}

inline void EventTarget::removeListener(const EventArray& events, EventListener* listener)
{
    for (int i = 0; i < events.size(); ++i)
        removeListener(events[i], listener);
}

inline void EventTarget::notify(EventId event, const EventParams& params)
{
    auto& listeners = m_listeners[event];

    for (auto it = listeners.begin(); it != listeners.end(); ++it)
        (*it)->onEvent(event, params);
}

} // ns mcr
