#include "Universe.h"
#include "SimpleMeshImporter.h"

namespace mcr          {
namespace gfx          {
namespace experimental {

namespace {
const GLenum g_attribTypesTr[] =
{
    GL_BYTE,  GL_UNSIGNED_BYTE,
    GL_SHORT, GL_UNSIGNED_SHORT,
    GL_INT,   GL_UNSIGNED_INT,
    GL_FLOAT, GL_DOUBLE
};
} // ns

rcptr<IMeshImporter> createSimpleMeshLoader()
{
    return new SimpleMeshImporter;
}

bool SimpleMeshImportTask::estimate(MeshInfo& infoOut)
{
    if (m_file->read(m_header) != sizeof(m_header))
        return false;

    if (!m_header.checkSignature() || m_header.version != SIMPLE_MESH_VERSION)
        return false;

    auto attribs = new SimpleMesh::VertexAttribute[m_header.numAttributes];

    m_file->seek(m_header.attributeDataOffset);
    m_file->read(attribs, m_header.numAttributes);

    for (uint i = 0; i < m_header.numAttributes; ++i)
        m_info.vertexFormat.addAttrib(g_attribTypesTr[attribs[i].type], attribs[i].length);

    delete [] attribs;

    m_info.numVertices = m_header.numVertices;
    m_info.numIndices  = m_header.numIndices;

    infoOut = m_info;
    return true;
}

bool SimpleMeshImportTask::import(const VertexFormat& bufferFormat, Mesh& mesh)
{
    // not capable of converting
    if (m_info.vertexFormat.numAttribs() != bufferFormat.numAttribs())
        return false;

    for (uint i = 0; i < bufferFormat.numAttribs(); ++i)
    {
        auto &myAttrib  = m_info.vertexFormat.attrib(i),
             &refAttrib = bufferFormat.attrib(i);

        if (myAttrib.type != refAttrib.type || myAttrib.length != refAttrib.length)
            return false;
    }

    size_t written = 0;

    auto vertices = mesh.mapVertices(GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
    auto indices  = mesh.mapIndices (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);

    m_file->seek(m_header.vertexDataOffset);
    written += m_file->read(vertices, m_header.numVertices * m_header.vertexSize);

    m_file->seek(m_header.indexDataOffset);
    written += m_file->read(indices, m_header.numIndices);

    mesh.unmapVertices();
    mesh.unmapIndices();

    return written == m_header.numVertices * m_header.vertexSize
                    + m_header.numIndices  * sizeof(uint);
}

} // ns experimental
} // ns gfx
} // ns mcr
