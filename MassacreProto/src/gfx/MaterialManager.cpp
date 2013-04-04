#include "Universe.h"
#include "gfx/MaterialManager.h"

#include <set>
#include <boost/algorithm/string/case_conv.hpp>

#pragma warning(push)
#pragma warning(disable: 4146)
#include <yaml-cpp/yaml.h>
#pragma warning(pop)

using namespace mcr;
using namespace gfx;

Texture* MaterialManager::getTexture(const char* filename)
{
    std::string path;
    auto file = m_fs->openFile(filename, &path);

    if (path.empty())
        return nullptr;

    auto& tex = m_textures[path];
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
        mtl = Material::create(0);
        _parseMaterial(mtl, file);
    }

    return mtl;
}

ShaderProgram* MaterialManager::getShaderProgram(const char* vsFilename, const char* fsFilename)
{
    std::set<std::string> filenames;

    filenames.insert(vsFilename);
    filenames.insert(fsFilename);

    return _getShaderProgram(filenames);
}

ShaderProgram* MaterialManager::getShaderProgram(
    const char* vsFilename, const char* gsFilename, const char* fsFilename)
{
    std::set<std::string> filenames;

    filenames.insert(vsFilename);
    filenames.insert(gsFilename);
    filenames.insert(fsFilename);

    return _getShaderProgram(filenames);
}

void MaterialManager::clear()
{
    m_materials.clear();
    m_programs.clear();
    m_shaders.clear();
    m_textures.clear();
}

void MaterialManager::removeUnused()
{
    _dropAll(m_materials);
    _dropAll(m_programs);
    _dropAll(m_shaders);
    _dropAll(m_textures);

    _grabAll(m_textures);
    _grabAll(m_shaders);
    _grabAll(m_programs);
    _grabAll(m_materials);
}

std::map<std::string, rect> MaterialManager::parseAtlasTMP(const char* filename)
{
    std::map<std::string, rect> result;

    FileStream stream(m_fs->openFile(filename));
    YAML::Parser parser(std::istream(&stream, false));

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

Shader* MaterialManager::_getShader(const char* filename, std::string& path)
{
    auto file = m_fs->openFile(filename, &path);

    if (path.empty())
        return nullptr;

    auto& shdr = m_shaders[path];
    if (!shdr)
        shdr = Shader::createFromFile(file);

    return shdr;
}

ShaderProgram* MaterialManager::_getShaderProgram(const std::set<std::string>& filenames)
{
    std::vector<Shader*> shaders;
    std::string name;

    shaders.reserve(filenames.size());

    for (auto it = filenames.begin(); it != filenames.end(); ++it)
    {
        std::string path;

        auto shader = _getShader(it->c_str(), path);
        if (!shader)
            return nullptr;

        shaders.push_back(shader);

        if (name.size())
            name += '|';

        name += path;
    }

    auto& prog = m_programs[name];
    if (!prog)
    {
        prog = ShaderProgram::create();

        for (auto it = shaders.begin(); it != shaders.end(); ++it)
            prog->attach(*it);

        prog->link();
    }

    return prog;
}

void MaterialManager::_parseMaterial(Material* mtl, IFile* file)
{
    static const std::pair<std::string, int> literalArray[] =
    {
        std::make_pair("never",    GL_NEVER),
        std::make_pair("less",     GL_LESS),
        std::make_pair("equal",    GL_EQUAL),
        std::make_pair("lequal",   GL_LEQUAL),
        std::make_pair("greater",  GL_GREATER),
        std::make_pair("notequal", GL_NOTEQUAL),
        std::make_pair("gequal",   GL_GEQUAL),
        std::make_pair("always",   GL_ALWAYS),
        std::make_pair("one",      GL_ONE),
        std::make_pair("zero",     GL_ZERO),

        std::make_pair("srccolor",         GL_SRC_COLOR),
        std::make_pair("oneminussrccolor", GL_ONE_MINUS_SRC_COLOR),
        std::make_pair("srcalpha",         GL_SRC_ALPHA),
        std::make_pair("oneminussrcalpha", GL_ONE_MINUS_SRC_ALPHA),
        std::make_pair("dstalpha",         GL_DST_ALPHA),
        std::make_pair("oneminusdstalpha", GL_ONE_MINUS_DST_ALPHA),
        std::make_pair("dstcolor",         GL_DST_COLOR),
        std::make_pair("oneminusdstcolor", GL_ONE_MINUS_DST_COLOR),
        std::make_pair("srcalphasaturate", GL_SRC_ALPHA_SATURATE),
    };

    static const std::map<std::string, int> literals(
        literalArray, literalArray + sizeof(literalArray) / sizeof(literalArray[0]));

    auto dir = Resource::create(file->fs(), Path::dir(file->filename()).c_str());

    FileStream stream(file);
    YAML::Parser parser(std::istream(&stream, false));

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
                    rs.depthFunc = lit->second;
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
                    rs.blendFunc[0] = lit->second;
                else
                    it.second()[0] >> rs.blendFunc[0];

                lit = literals.find(fn[1]);
                if (lit != literals.end())
                    rs.blendFunc[1] = lit->second;
                else
                    it.second()[1] >> rs.blendFunc[1];
            }
            else if (key == "passhint")
            {
                int hint;
                it.second() >> hint;
                mtl->setPassHint(hint);
            }
            else if (key == "shaders")
            {
                std::set<std::string> filenames;

                for (auto nodeIt = it.second().begin(); nodeIt != it.second().end(); ++nodeIt)
                    filenames.insert(nodeIt->to<std::string>());

                //mtl->setShader(_getShaderProgram(filenames));
                mtl->initProgram(getShader((*filenames.begin()).c_str()),
                                 getShader((*++filenames.begin()).c_str()));  // HACK
            }
            else if (key == "textures")
            {
                auto& textures = it.second();

                assert(textures.size() < 256);

                mtl->setNumTextures((byte) textures.size());

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
