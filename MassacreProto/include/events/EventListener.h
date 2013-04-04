#pragma once

#include "Event.h"
#include "EventParams.h"

namespace mcr {

class EventListener
{
public:
    virtual ~EventListener() = 0;
    virtual void onEvent(EventId event, const EventParams& params) = 0;
};

inline EventListener::~EventListener() {}

} // ns mcr
