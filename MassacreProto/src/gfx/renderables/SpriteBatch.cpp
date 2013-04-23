#include "Universe.h"
#include <mcr/gfx/renderables/SpriteBatch.h>

namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// Internal sprite class

class SpriteBatch::Sprite: public ISprite
{
public:
    Sprite(SpriteBatch* batch, uint index):
        m_batch(batch), m_index(index) {}

    ~Sprite()
    {
        std::swap(
            m_batch->m_sprites[m_index],
            m_batch->m_sprites[m_batch->m_sprites.size() - 1]);

        // TODO: swap index blocks

        m_batch->m_sprites.pop_back();
        m_batch->m_atom.size -= 6;
    }


    const mat4& transform() const
    {
        return m_data->transform;
    }
    void setTransform(const mat4& tf)
    {
        m_data->transform = tf;
        invalidate();
    }

    const rect& baseRect() const
    {
        return m_data->baseRect;
    }
    void setBaseRect(const rect& baseRect)
    {
        m_data->baseRect = baseRect;
        invalidate();
    }

    const rect& texRect() const
    {
        return m_data->texRect;
    }
    void setTexRect(const rect& texRect)
    {
        m_data->texRect = texRect;
        invalidate();
    }

    const void* userAttribData(uint i) const
    {
        return &m_data->userData[m_batch->m_format->attrib(i).offset];
    }
    void setUserAttribData(uint i, const void* data)
    {
        auto& attrib = m_batch->m_format->attrib(i);
        memcpy(
            &m_data->userData[attrib.offset], data,
            attrib.length * attrib.type.size());
    }

    void invalidate()
    {
        if (!m_data->dirty)
        {
            m_data->dirty = true;
            ++m_batch->m_dirtySprites;
        }
    }

    void updateDataPtr()
    {
        m_data = &m_batch->m_sprites[m_index];
    }

    SpriteBatch* m_batch;
    SpriteBatch::SpriteData* m_data;
    uint m_index;
};


//////////////////////////////////////////////////////////////////////////
// Finally, methods

SpriteBatch::SpriteBatch(uint maxSprites, const VertexFormat& fmt):

    m_dirtySprites(0), m_maxSprites(maxSprites)
{
    auto& posAttrib = *fmt.attribBySemantic(PositionSemantic);
    auto& tcAttrib  = *fmt.attribBySemantic(TexcoordSemantic);

    assert(&posAttrib && posAttrib.length == 3 && posAttrib.type == GType::Float);
    assert(&tcAttrib  && tcAttrib .length == 2 && tcAttrib .type == GType::Float);


    m_vertexSize = fmt.stride();
    m_vertexPositionOffset = posAttrib.offset;
    m_vertexTexcoordOffset = tcAttrib.offset;


    static const uint numVertices = 4, numIndices = 6;
    static const uint indexRef[numIndices] = {0,1,2, 0,2,3};

    m_buffer = VertexArray::create(
        m_maxSprites * numVertices * m_vertexSize,
        m_maxSprites * sizeof(indexRef));

    m_buffer->setFormat(fmt);
    m_format = &m_buffer->format();

    auto indices = (uint*) m_buffer->indices()->map(GL_MAP_WRITE_BIT);

    for (uint i = 0; i < numIndices * m_maxSprites; ++i)
        *indices++ = numVertices * (i / numIndices) + indexRef[i % numIndices];

    m_buffer->indices()->unmap();


    m_atom.vao   = m_buffer->handle();
    m_atom.start = 0;
    m_atom.size  = 0;
}

SpriteBatch::~SpriteBatch()
{
    BOOST_FOREACH (auto& sprite, m_sprites)
    {
        delete [] sprite.userData;
        delete sprite.shortcut;
    }
}


ISprite* SpriteBatch::addSprite(const rect& baseRect, const rect& texRect, const mat4& transform)
{
    SpriteData sprite =
    {
        transform, baseRect, texRect,
        new byte[m_vertexSize],
        new Sprite(this, m_sprites.size()),
        true
    };

    m_sprites.emplace_back(sprite);
    m_sprites.back().shortcut->updateDataPtr();

    ++m_dirtySprites;

    m_atom.size += 6;

    return m_sprites.back().shortcut;
}

void SpriteBatch::applyChanges()
{
    if (!m_dirtySprites || !m_atom.material)
        return;

    auto vertices = (byte*) m_buffer->vertices()->map(GL_MAP_WRITE_BIT);

    const vec2 texSize = m_atom.material->texture(0)->size();

    for (uint i = 0; i < m_sprites.size(); ++i)
    {
        auto& sprite = m_sprites[i];

        if (!sprite.dirty)
            continue;

        auto v = &vertices[4 * i * m_vertexSize];

        auto& br = sprite.baseRect;
        auto& tf = sprite.transform;
        auto  tc = sprite.texRect / texSize;
        auto& p  = tf.translation();

        memcpy(v, sprite.userData, m_vertexSize);
        *((vec3*)(v + m_vertexPositionOffset)) = vec3(br.topLeft(),  0) * tf + p;
        *((vec2*)(v + m_vertexTexcoordOffset)) = tc.topLeft();
        v += m_vertexSize;

        memcpy(v, sprite.userData, m_vertexSize);
        *((vec3*)(v + m_vertexPositionOffset)) = vec3(br.topRight(), 0) * tf + p;
        *((vec2*)(v + m_vertexTexcoordOffset)) = tc.topRight();
        v += m_vertexSize;

        memcpy(v, sprite.userData, m_vertexSize);
        *((vec3*)(v + m_vertexPositionOffset)) = vec3(br.botRight(), 0) * tf + p;
        *((vec2*)(v + m_vertexTexcoordOffset)) = tc.botRight();
        v += m_vertexSize;

        memcpy(v, sprite.userData, m_vertexSize);
        *((vec3*)(v + m_vertexPositionOffset)) = vec3(br.botLeft(),  0) * tf + p;
        *((vec2*)(v + m_vertexTexcoordOffset)) = tc.botLeft();

        sprite.dirty = false;

        if (!--m_dirtySprites)
            break;
    }

    m_buffer->vertices()->unmap();
}

} // ns gfx
} // ns mcr
