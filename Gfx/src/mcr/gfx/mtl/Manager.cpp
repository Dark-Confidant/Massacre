#include "Universe.h"
#include <mcr/gfx/mtl/Manager.h>

#include <istream>
#include <boost/algorithm/string/case_conv.hpp>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4146)
#   include <yaml-cpp/yaml.h>
#   pragma warning(pop)
#else
#   include <yaml-cpp/yaml.h>
#endif

#include <mcr/io/InputStream.h>
#include "ShaderPreprocessor.h"

namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Init

void Manager::_init()
{
    m_preprocessor = new ShaderPreprocessor(this);
}

void Manager::_destroy()
{
    delete m_preprocessor;
}

Manager::ParamBufferData::ParamBufferData():
    nextFreeBinding(0)
{
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, (GLint*) &numBindings);
}

Manager::TextureData::TextureData():
    nextFreeUnit(0)
{
    GLint numUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numUnits);

    units.resize((std::size_t) numUnits);
}


//////////////////////////////////////////////////////////////////////////
// Cached getters

Texture* Manager::getTexture(const std::string& filename)
{
    auto file = m_fs->openReader(filename.c_str());
    if (!file)
        return nullptr;

    auto& tex = m_tex.textures[filename];
    if (!tex)
    {
        tex = Texture::create();
        tex->load(file);
    }

    return tex;
}

Shader* Manager::getShader(const std::string& filename)
{
    auto file = m_fs->openReader(filename.c_str());
    if (!file)
        return nullptr;

    auto& shader = m_shaders[filename];
    if (!shader)
    {
        auto type = Shader::Vertex;
    
        switch (io::Path::ext(file->filename())[0])
        {
        case 'g': type = Shader::Geometry; break;
        case 'f': type = Shader::Fragment;
        }

        shader = Shader::create(type);

        shader->setPreprocessor(m_preprocessor);
        shader->setSourceFromStream(file);
    }

    return shader;
}

Material* Manager::getMaterial(const std::string& filename)
{
    auto file = m_fs->openReader(filename.c_str());
    if (!file)
        return nullptr;

    auto& material = m_materials[filename];
    if (!material)
    {
        material = Material::create(this);
        _parseMaterial(material, file);
    }

    return material;
}


//////////////////////////////////////////////////////////////////////////
// Cleanup interface

void Manager::clear()
{
    m_materials.clear();
    m_shaders.clear();
    m_tex.textures.clear();
}

void Manager::removeUnused()
{
    _dropAll(m_materials);
    _dropAll(m_shaders);
    _dropAll(m_tex.textures);

    _grabAll(m_tex.textures);
    _grabAll(m_shaders);
    _grabAll(m_materials);
}


//////////////////////////////////////////////////////////////////////////
// Internals

void Manager::_parseMaterial(Material* material, io::IReader* stream)
{
    static const std::pair<std::string, uint> literalArray[] =
    {
        std::make_pair("never",    DepthFn::Never),
        std::make_pair("less",     DepthFn::Less),
        std::make_pair("equal",    DepthFn::Equal),
        std::make_pair("lequal",   DepthFn::LEqual),
        std::make_pair("greater",  DepthFn::Greater),
        std::make_pair("notequal", DepthFn::NotEqual),
        std::make_pair("gequal",   DepthFn::GEqual),
        std::make_pair("always",   DepthFn::Always),

        std::make_pair("one",               BlendFn::One),
        std::make_pair("zero",              BlendFn::Zero),
        std::make_pair("srccolor",          BlendFn::SrcColor),
        std::make_pair("oneminussrccolor",  BlendFn::OneMinusSrcColor),
        std::make_pair("srcalpha",          BlendFn::SrcAlpha),
        std::make_pair("oneminussrcalpha",  BlendFn::OneMinusSrcAlpha),
        std::make_pair("dstalpha",          BlendFn::DstAlpha),
        std::make_pair("oneminusdstalpha",  BlendFn::OneMinusDstAlpha),
        std::make_pair("dstcolor",          BlendFn::DstColor),
        std::make_pair("oneminusdstcolor",  BlendFn::OneMinusDstColor),
        std::make_pair("srcalphasaturate",  BlendFn::SrcAlphaSaturate),
    };

    static const std::map<std::string, int> literals(
        literalArray, literalArray + sizeof(literalArray) / sizeof(literalArray[0]));

    io::InputStream istream(stream);
    std::istream in(&istream);
    YAML::Parser parser(in);

    YAML::Node doc;
    parser.GetNextDocument(doc);

    if (doc.Type() == YAML::NodeType::Map)
    {
        RenderState rs;

        std::string key;
        for (auto it = doc.begin(); it != doc.end(); ++it)
        {
            it.first() >> key;
            boost::to_lower(key);

            if      (key == "depthtest")     it.second() >> rs.depthTest;
            else if (key == "depthwrite")    it.second() >> rs.depthWrite;
            else if (key == "alphatest")     it.second() >> rs.alphaTest;
            else if (key == "blend")         it.second() >> rs.blend;
            else if (key == "cullface")      it.second() >> rs.cullFace;
            else if (key == "polygonoffset") it.second() >> rs.polygonOffset;
            else if (key == "depthfunc")
            {
                std::string fn;

                it.second() >> fn;
                boost::to_lower(fn);

                auto lit = literals.find(fn);
                if (lit != literals.end())
                    rs.depthFunc = DepthFn(lit->second);
            }
            else if (key == "blendfunc")
            {
                std::string fn[2];

                it.second()[0] >> fn[0];
                it.second()[1] >> fn[1];

                boost::to_lower(fn[0]);
                boost::to_lower(fn[1]);

                auto lit = literals.find(fn[0]);
                if (lit != literals.end())
                    rs.blendFunc.srcFactor = BlendFn::Factor(lit->second);

                lit = literals.find(fn[1]);
                if (lit != literals.end())
                    rs.blendFunc.dstFactor = BlendFn::Factor(lit->second);
            }
            else if (key == "passhint")
            {
                int hint;
                it.second() >> hint;
                material->setPassHint(hint);
            }
            else if (key == "shaders")
            {
                gfx::mtl::ShaderList shaders;

                for (auto nodeIt = it.second().begin(); nodeIt != it.second().end(); ++nodeIt)
                    shaders.add(getShader(nodeIt->to<std::string>().c_str()));

                material->setShaders(shaders);
            }
            else if (key == "textures")
            {
                auto& textures = it.second();

                assert(textures.size() < 256);

                std::string tex;
                for (byte i = 0; i < textures.size(); ++i)
                {
                    textures[size_t(i)] >> tex;
                    material->setTexture(i, getTexture(tex.c_str()));
                }
            }
        }

        material->setRenderState(rs);
    }
}

} // ns mtl
} // ns gfx
} // ns mcr
