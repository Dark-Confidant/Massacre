#include "Universe.h"
#include "gfx/VertexArray.h"

#include <set>

using namespace mcr;
using namespace gfx;

rcptr<VertexArray> VertexArray::create()
{
    return new VertexArray;
}

rcptr<VertexArray> VertexArray::create(uint vertexDataSize, uint indexDataSize)
{
    auto va = new VertexArray;

    va->m_vertices->init(vertexDataSize, GL_STATIC_DRAW);
    va->m_indices->init(indexDataSize, GL_STATIC_DRAW);

    return va;
}

rcptr<VertexArray> VertexArray::create(
    const void* vertexData, uint vertexDataSize,
    const void* indexData, uint indexDataSize)
{
    auto va = new VertexArray;

    va->m_vertices->init(vertexData, vertexDataSize, GL_STATIC_DRAW);
    va->m_indices->init(indexData, indexDataSize, GL_STATIC_DRAW);

    return va;
}


VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_handle);
    glBindVertexArray(m_handle);

    m_vertices = GBuffer::create(GL_ARRAY_BUFFER);
    m_indices = GBuffer::create(GL_ELEMENT_ARRAY_BUFFER);
}

VertexArray::~VertexArray()
{
    m_indices = nullptr;
    m_vertices = nullptr;

    glDeleteVertexArrays(1, &m_handle);
}


void VertexArray::setFormat(const VertexFormat& fmt)
{
    m_format = fmt;

    glBindVertexArray(m_handle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertices->handle());

    for (uint i = 0; i < fmt.numAttribs(); ++i)
    {
        auto& attrib = fmt.attrib(i);

        if (!attrib.length)
            continue;

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,
            attrib.length, attrib.type, GL_FALSE,
            fmt.stride(), (const GLvoid*) attrib.offset);
    }
}

void VertexArray::transformAttribs(uint attribIdx, const mat4& tf, uint startVertex, uint numVertices)
{
    if (attribIdx >= m_format.numAttribs() || m_format.stride() == 0)
        return;

    auto& attrib = m_format.attrib(attribIdx);
    // TODO: type check

    const uint vertexSize       = m_format.stride();
    const uint totalNumVertices = m_vertices->capacity() / vertexSize;

    if (startVertex >= totalNumVertices)
        return;

    numVertices = std::min(numVertices, totalNumVertices - startVertex);

    auto v = (byte*) m_vertices->map(
        startVertex * vertexSize,
        numVertices * vertexSize,
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT) + attrib.offset;

    vec4 ref(0, 0, 0, 1);

    for (; numVertices--; v += vertexSize)
    {
        for (uint comp = 0; comp != attrib.length; ++comp)
            static_cast<float*>(ref)[comp] = reinterpret_cast<const float*>(v)[comp];

        ref *= tf;

        for (uint comp = 0; comp != attrib.length; ++comp)
            reinterpret_cast<float*>(v)[comp] = ref[comp];
    }

    m_vertices->unmap();
}

void VertexArray::transformAttribs(uint attribIdx, const mat4& tf)
{
    if (m_format.stride() != 0)
        transformAttribs(attribIdx, tf, 0, m_vertices->capacity() / m_format.stride());
}

void VertexArray::transformAttribsIndexed(uint attribIdx, const mat4& tf, uint startIndex, uint numIndices)
{
    if (attribIdx >= m_format.numAttribs() || m_format.stride() == 0)
        return;

    auto& attrib = m_format.attrib(attribIdx);
    // TODO: type check

    const uint vertexSize      = m_format.stride();
    const uint totalNumIndices = m_indices->capacity() / sizeof(uint);

    if (startIndex >= totalNumIndices)
        return;

    numIndices = std::min(numIndices, totalNumIndices - startIndex);

    auto ind = (const uint*) m_indices->map(
        startIndex * sizeof(uint),
        numIndices * sizeof(uint),
        GL_MAP_READ_BIT);

    auto vert = (byte*) m_vertices->map(GL_MAP_WRITE_BIT)
              + attrib.offset;

    std::set<uint> transformed;

    vec4 ref(0, 0, 0, 1);

    for (; numIndices--; ++ind)
    {
        if (transformed.find(*ind) != transformed.end())
            continue;

        transformed.insert(*ind);

        auto attr = vert + vertexSize * *ind;

        for (uint comp = 0; comp != attrib.length; ++comp)
            static_cast<float*>(ref)[comp] = reinterpret_cast<const float*>(attr)[comp];

        ref *= tf;

        for (uint comp = 0; comp != attrib.length; ++comp)
            reinterpret_cast<float*>(attr)[comp] = ref[comp];
    }

    m_vertices->unmap();
    m_indices->unmap();
}
