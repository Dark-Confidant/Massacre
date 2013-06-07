#include "Universe.h"
#include <mcr/gfx/mtl/Manager.h>

#include <istream>
#include <boost/algorithm/string/case_conv.hpp>
#include <mcr/io/LineParser.h>
#include "ShaderPreprocessor.h"

// TODO: rethink, rewrite

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

Manager::ParamBufferData::~ParamBufferData() {}

Manager::TextureData::TextureData():
    nextFreeUnit(0)
{
    GLint numUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numUnits);

    units.resize((std::size_t) numUnits);
}

Manager::TextureData::~TextureData() {}


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
    static std::map<std::string, bool RenderState::*>   stateCommands;
    static std::map<std::string, DepthFn>               depthFnLiterals;
    static std::map<std::string, BlendFn::Factor>       blendFnLiterals;

    if (stateCommands.empty())
    {
        stateCommands["depthtest"]     = &RenderState::depthTest;
        stateCommands["depthwrite"]    = &RenderState::depthWrite;
        stateCommands["alphatest"]     = &RenderState::alphaTest;
        stateCommands["blend"]         = &RenderState::blend;
        stateCommands["cullface"]      = &RenderState::cullFace;
        stateCommands["polygonoffset"] = &RenderState::polygonOffset;
    }

    if (depthFnLiterals.empty())
    {
        depthFnLiterals["never"]    = DepthFn::Never;
        depthFnLiterals["less"]     = DepthFn::Less;
        depthFnLiterals["equal"]    = DepthFn::Equal;
        depthFnLiterals["lequal"]   = DepthFn::LEqual;
        depthFnLiterals["greater"]  = DepthFn::Greater;
        depthFnLiterals["notequal"] = DepthFn::NotEqual;
        depthFnLiterals["gequal"]   = DepthFn::GEqual;
        depthFnLiterals["always"]   = DepthFn::Always;
    }

    if (blendFnLiterals.empty())
    {
        blendFnLiterals["one"]               = BlendFn::One;
        blendFnLiterals["zero"]              = BlendFn::Zero;
        blendFnLiterals["srccolor"]          = BlendFn::SrcColor;
        blendFnLiterals["oneminussrccolor"]  = BlendFn::OneMinusSrcColor;
        blendFnLiterals["srcalpha"]          = BlendFn::SrcAlpha;
        blendFnLiterals["oneminussrcalpha"]  = BlendFn::OneMinusSrcAlpha;
        blendFnLiterals["dstalpha"]          = BlendFn::DstAlpha;
        blendFnLiterals["oneminusdstalpha"]  = BlendFn::OneMinusDstAlpha;
        blendFnLiterals["dstcolor"]          = BlendFn::DstColor;
        blendFnLiterals["oneminusdstcolor"]  = BlendFn::OneMinusDstColor;
        blendFnLiterals["srcalphasaturate"]  = BlendFn::SrcAlphaSaturate;
    }

    enum {States, Textures, Shaders} mode = States;

    RenderState renderState;
    ShaderList shaders;
    byte iTexture = 0;

    char key[64] = {}, value[256] = {};
    int tokens = 0;

    for (io::LineParser parser(stream); parser.readLine();)
    {
        if (!parser.indent())
        {
            if (mode == Shaders)
                material->setShaders(shaders);

            mode = States;
        }

        switch (mode)
        {
        case States:
            tokens = std::sscanf(parser.line().c_str(), "%63[^:] : %255s", key, value);
            if (tokens == 2)
            {
                boost::to_lower(key);
                boost::to_lower(value);

                auto stateCmdIt = stateCommands.find(key);
                if (stateCmdIt != stateCommands.end())
                    renderState.*stateCmdIt->second = strcmp(value, "true") == 0;

                else if (strcmp(key, "depthfunc") == 0)
                {
                    auto depthFnIt = depthFnLiterals.find(value);
                    if (depthFnIt != depthFnLiterals.end())
                        renderState.depthFunc = depthFnIt->second;
                }
                else if (strcmp(key, "blendfunc") == 0)
                {
                    char srcFactor[32], dstFactor[32];
                    if (std::sscanf("[ %31[^ 0xa0\t,] , %31[^ 0xa0\t\\] ]", srcFactor, dstFactor) == 2)
                    {
                        auto blendFnIt = blendFnLiterals.find(srcFactor);
                        if (blendFnIt != blendFnLiterals.end())
                            renderState.blendFunc.srcFactor = blendFnIt->second;

                        blendFnIt = blendFnLiterals.find(dstFactor);
                        if (blendFnIt != blendFnLiterals.end())
                            renderState.blendFunc.dstFactor = blendFnIt->second;
                    }
                }
                else if (strcmp(key, "passhint") == 0)
                    material->setPassHint(atoi(value));
            }
            else if (tokens == 1)
            {
                boost::to_lower(key);

                if (strcmp(key, "shaders") == 0)
                    mode = Shaders;

                else if (strcmp(key, "textures") == 0)
                    mode = Textures;
            }
            break;

        case Shaders:
            if (std::sscanf(parser.line().c_str(), " - %255s", value) == 1)
                shaders.add(getShader(value));
            break;

        case Textures:
            if (std::sscanf(parser.line().c_str(), " - %255s", value) == 1)
                material->setTexture(iTexture++, getTexture(value));
            break;
        }
    }

    material->setRenderState(renderState);
}

} // ns mtl
} // ns gfx
} // ns mcr
