#pragma once

#include "gfx/renderables/Renderable.h"
#include "gfx/VertexArray.h"
#include "gfx/ISprite.h"

namespace mcr {
namespace gfx {

class SpriteBatch: public Renderable
{
public:
    enum { PositionSemantic = 'p', TexcoordSemantic = 't' };

    static rcptr<SpriteBatch> create(
        uint maxSprites,
        const VertexFormat& fmt = "p3f _ t2f")
    {
        return new SpriteBatch(maxSprites, fmt);
    }

    ISprite* addSprite(
        const rect& baseRect,
        const rect& texRect,
        const mat4& transform = mat4::I());

    ISprite* addSprite(
        const rect& texRect,
        const mat4& transform = mat4::I())
    {
        return addSprite(texRect, texRect, transform);
    }

    uint numSprites() const { return (uint) m_sprites.size(); }
    uint maxSprites() const { return m_maxSprites; }

    void applyChanges();

    Material* material() const
    {
        return m_atom.material;
    }
    void setMaterial(Material* mtl)
    {
        m_atom.material = mtl;
        notify(AtomInvalidated, &m_atom);
    }

    uint numAtoms() const { return 1; }
    const RenderAtom* atom(uint) const { return &m_atom; }

protected:
    class Sprite;
    friend class Sprite;

    struct SpriteData
    {
        mat4 transform;
        rect baseRect, texRect;
        byte* userData;
        Sprite* shortcut;
        bool dirty;
    };

    SpriteBatch(uint maxSprites, const VertexFormat& fmt);
    ~SpriteBatch();

    SpriteBatch(const SpriteBatch&);
    SpriteBatch& operator=(const SpriteBatch&);

    RenderAtom m_atom;

    rcptr<VertexArray> m_buffer;
    const VertexFormat* m_format;
    uint m_vertexSize, m_vertexPositionOffset, m_vertexTexcoordOffset;

    std::vector<SpriteData> m_sprites;
    uint m_dirtySprites;
    const uint m_maxSprites;
};

} // ns gfx
} // ns mcr
