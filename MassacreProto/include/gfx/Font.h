#pragma once

#include <unordered_map>
#include "Texture.h"

namespace mcr {
namespace gfx {

class FontSize;

class Font: public RefCounted
{
public:
    static rcptr<Font> createFromFile(IFile* file);

    void load(IFile* file);

    FontSize* size(uint i) const;
    FontSize* closestSize(float i) const;

protected:
    Font();
    ~Font();

    std::unordered_map<uint, rcptr<Texture>> m_sizes;

    friend struct Impl;
    struct Impl;
    Impl* m_impl;
};

} // ns gfx
} // ns mcr
