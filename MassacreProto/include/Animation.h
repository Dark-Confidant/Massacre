#pragma once

namespace mcr {

class Animation
{
public:
    virtual ~Animation() {}
    virtual void setFrame(float frame) = 0;
};

} // ns mcr
