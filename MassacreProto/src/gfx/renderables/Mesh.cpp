#include "Universe.h"
#include "gfx/renderables/Mesh.h"

#include <map>
#include <fstream>
#include <SimpleMesh3.h>

using namespace mcr;
using namespace gfx;
using namespace SimpleMesh;

namespace {

Texture* getCachedTexture(FileSystem* fs, const std::string& name)
{
    static std::map<std::string, rcptr<Texture>> cache;

    auto& tex = cache[name];
    if (!tex)
        tex = Texture::createFromFile(fs->openFile(name.c_str()));

    return tex;
}

} // ns


rcptr<Mesh> Mesh::create()
{
    return new Mesh;
}

rcptr<Mesh> Mesh::createFromFile(IFile* file)
{
    auto mesh = new Mesh;
    mesh->load(file);

    return mesh;
}


void Mesh::load(IFile* file)
{
    Header header;
    file->read(header);

    if (!header.checkSignature())
        return;

    if (header.version != SIMPLE_MESH_VERSION)
        return;


    // reading attributes

    VertexFormat fmt;

    auto attribs = new VertexAttribute[header.numAttributes];

    file->seek(header.attributeDataOffset);
    file->read(attribs, header.numAttributes);

    for (uint i = 0; i < header.numAttributes; ++i)
        fmt.addAttrib(GType(GType::SByte + attribs[i].type), attribs[i].length); // sic!

    delete [] attribs;

    m_buffer->setFormat(fmt);


    // read vertices

    const uint vertexDataSize = header.vertexSize * header.numVertices;

    m_buffer->vertices()->init(vertexDataSize, GL_STATIC_DRAW);
    
    file->seek(header.vertexDataOffset);
    file->read(m_buffer->vertices()->map(GL_MAP_WRITE_BIT), vertexDataSize);

    m_buffer->vertices()->unmap();


    // read indices

    const uint indexDataSize = sizeof(uint) * header.numIndices;

    m_buffer->indices()->init(indexDataSize, GL_STATIC_DRAW);

    file->seek(header.indexDataOffset);
    file->read(m_buffer->indices()->map(GL_MAP_WRITE_BIT), indexDataSize);

    m_buffer->indices()->unmap();


    // read parts
    auto dir = Resource::create(file->fs(), Path::dir(file->filename()).c_str());

    m_parts.resize(header.numParts);

    file->seek(header.partDataOffset);

    for (size_t i = 0; i < header.numParts; ++i)
    {
        using namespace MaterialState;

        Part part;
        file->read(part);

        auto& mpart = m_parts[i];

        mpart.vao   = m_buffer->handle();
        mpart.start = part.start;
        mpart.size  = part.size;

        auto& params = part.material;
        auto& mtl    = mpart.material = Material::create(part.numTextures);

        RenderState rs;

        rs.alphaTest  = params.getState(AlphaTest);
        rs.depthTest  = params.getState(DepthTest);
        rs.depthWrite = params.getState(DepthWrite);
        rs.blend      = params.getState(Blend);
        rs.cullFace   = params.getState(CullFace);

        rs.depthFunc  = params.depthFunc + GL_NEVER;
        rs.blendFunc  = ivec2(params.blendSrcFunc, params.blendDstFunc)
                      + ivec2(GL_SRC_COLOR);

        mtl->setRenderState(rs);

        for (byte i = 0; i < part.numTextures; ++i)
        {
            std::string texName;
            file->readString0(texName);

            mtl->setTexture(i, getCachedTexture(file->fs(), texName));
        }
    }
}

void Mesh::optimizeAtomsTMP()
{
    std::map<Material*, std::vector<RenderAtom>> byMtl;

    BOOST_FOREACH (auto& part, m_parts)
        byMtl[part.material].push_back(part);

    m_parts.clear();

    BOOST_FOREACH (auto& pair, byMtl)
    {
        auto& mtlAtoms = pair.second;

        std::sort(mtlAtoms.begin(), mtlAtoms.end(),
            [] (const RenderAtom& a, const RenderAtom& b) { return a.start < b.start; });

        uint prevAtomIdx = 0;
        for (uint atomIdx = 1; atomIdx != mtlAtoms.size();)
        {
            auto& prevAtom = mtlAtoms[prevAtomIdx];
            auto& atom     = mtlAtoms[atomIdx];

            if (atom.start == prevAtom.start + prevAtom.size)
            {
                prevAtom.size += atom.size;
                mtlAtoms.erase(mtlAtoms.begin() + atomIdx);
            }
            else
            {
                ++prevAtomIdx;
                ++atomIdx;
            }
        }

        BOOST_FOREACH (auto& atom, mtlAtoms)
            m_parts.push_back(atom);
    }
}
