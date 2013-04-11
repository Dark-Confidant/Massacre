#pragma once

namespace mcr {

class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}

private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};

} // ns mcr
