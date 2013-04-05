#pragma once

namespace mcr {

typedef uintptr_t EventId;

class Event
{
public:
    Event() {}
    operator EventId() const { return (uintptr_t) this; }
};

} // ns mcr
