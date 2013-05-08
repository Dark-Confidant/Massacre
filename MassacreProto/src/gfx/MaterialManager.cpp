#include "Universe.h"
#include <mcr/gfx/MaterialManager.h>

#include <set>
#include <istream>
#include <boost/algorithm/string/case_conv.hpp>

#include "ShaderPreprocessor.h"

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4146)
#   include <yaml-cpp/yaml.h>
#   pragma warning(pop)
#else
#   include <yaml-cpp/yaml.h>
#endif

namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// Init

void MaterialManager::_init()
{
    m_preprocessor = new ShaderPreprocessor(this);
}

void MaterialManager::_destroy()
{
    delete m_preprocessor;
}

MaterialManager::ParamBufferData::ParamBufferData():
    nextFreeBinding(0)
{
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, (GLint*) &numBindings);
}

MaterialManager::TextureData::TextureData():
    nextFreeUnit(0)
{
    GLint numUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numUnits);

    units.resize((std::size_t) numUnits);
}


//////////////////////////////////////////////////////////////////////////
// Cached getters

Texture* MaterialManager::getTexture(const char* filename)
{
    std::string path;
    auto file = m_fs->openFile(filename, &path);

    if (path.empty())
        return nullptr;

    auto& tex = m_tex.textures[path];
    if (!tex)
        tex = Texture::createFromFile(file);

    return tex;
}

Shader* MaterialManager::getShader(const char* filename)
{
    std::string path;
    return _getShader(filename, path);
}

Material* MaterialManager::getMaterial(const char* filename)
{
    std::string path;
    auto file = m_fs->openFile(filename, &path);

    if (path.empty())
        return nullptr;

    auto& mtl = m_materials[path];
    if (!mtl)
    {
        mtl = Material::create(this);
        _parseMaterial(mtl, file);
    }

    return mtl;
}


//////////////////////////////////////////////////////////////////////////
// Cleanup interface

void MaterialManager::clear()
{
    m_materials.clear();
    m_shaders.clear();
    m_tex.textures.clear();
}

void MaterialManager::removeUnused()
{
    _dropAll(m_materials);
    _dropAll(m_shaders);
    _dropAll(m_tex.textures);

    _grabAll(m_tex.textures);
    _grabAll(m_shaders);
    _grabAll(m_materials);
}


//////////////////////////////////////////////////////////////////////////
// Atlas stuff, to be removed

std::map<std::string, rect> MaterialManager::parseAtlasTMP(const char* filename)
{
    std::map<std::string, rect> result;

    FileStream stream(m_fs->openFile(filename));
    std::istream fin(&stream);
    YAML::Parser parser(fin);

    YAML::Node doc;
    parser.GetNextDocument(doc);

    auto& sizeNode = doc["meta"]["size"];

    ivec2 atlasSize;
    sizeNode["w"] >> atlasSize[0];
    sizeNode["h"] >> atlasSize[1];

    auto& frames = doc["frames"];

    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
        auto& frameNode = *it;

        std::string key;
        frameNode["filename"] >> key;

        auto& boundsNode = frameNode["frame"];

        ivec2 origin, size;
        boundsNode["x"] >> origin[0];
        boundsNode["y"] >> origin[1];
        boundsNode["w"] >> size[0];
        boundsNode["h"] >> size[1];

        origin.setY(atlasSize.y() - origin.y() - size.y());

        auto bounds = irect::bySize(origin, size);

        result[key] = rect(bounds) / vec2(atlasSize);
    }

    return result;
}


//////////////////////////////////////////////////////////////////////////
// Internals

Shader* MaterialManager::_getShader(const char* filename, std::string& path)
{
    auto file = m_fs->openFile(filename, &path);

    if (path.empty())
        return nullptr;

    auto& shdr = m_shaders[path];
    if (!shdr)
    {
        Shader::Type type = Shader::Vertex;
    
        switch (Path::ext(file->filename())[0])
        {
        case 'g': type = Shader::Geometry; break;
        case 'f': type = Shader::Fragment;
        }

        shdr = Shader::create(type);

        shdr->setPreprocessor(m_preprocessor);
        shdr->setSourceFromFile(file);
    }

    return shdr;
}

void MaterialManager::_parseMaterial(Material* mtl, IFile* file)
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

    auto dir = Resource::create(file->fs(), Path::dir(file->filename()).c_str());

    FileStream stream(file);
    std::istream fin(&stream);
    YAML::Parser parser(fin);

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
                mtl->setPassHint(hint);
            }
            else if (key == "shaders")
            {
                gfx::MaterialShaderSet shaders;

                for (auto nodeIt = it.second().begin(); nodeIt != it.second().end(); ++nodeIt)
                    shaders(getShader(nodeIt->to<std::string>().c_str()));

                mtl->setShaders(shaders);
            }
            else if (key == "textures")
            {
                auto& textures = it.second();

                assert(textures.size() < 256);

                std::string tex;
                for (byte i = 0; i < textures.size(); ++i)
                {
                    textures[size_t(i)] >> tex;
                    mtl->setTexture(i, getTexture(tex.c_str()));
                }
            }
        }

        mtl->setRenderState(rs);
    }
}

} // ns gfx
} // ns mcr
