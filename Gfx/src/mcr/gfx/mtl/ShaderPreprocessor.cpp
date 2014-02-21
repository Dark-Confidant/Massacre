#include "Universe.h"
#include "ShaderPreprocessor.h"

#include <sstream>
#include <cstring>
#include <algorithm>
#include <cctype>
#include "mcr/gfx/GLState.h"
#include <mcr/Log.h>
#include <mcr/gfx/mtl/Manager.h>

namespace mcr {
namespace gfx {
namespace mtl {

ShaderPreprocessor::ShaderPreprocessor(Manager* mgr): m_mm(mgr) {}
ShaderPreprocessor::~ShaderPreprocessor() {}

inline std::string buildBlockDef(const mtl::ParamBuffer* buffer)
{
    static const std::string s_paramTypeLiterals[] =
    {
        "float", "double", "int",   "uint",
        "vec2",  "dvec2",  "ivec2", "uvec2",
        "vec3",  "dvec3",  "ivec3", "uvec3",
        "vec4",  "dvec4",  "ivec4", "uvec4",
        "mat4",  "dmat4"
    };

    auto& params = buffer->layout().params;
    auto& name   = buffer->name();

    std::stringstream def;

    def << "layout(std140) uniform " << name << "Layout\n{\n";

    for (std::size_t i = 0; i < params.size(); ++i)
    {
        def
            << "    " << s_paramTypeLiterals[params[i].first]
            << ' ' << name
            << '_' << params[i].second
            << ";\n";
    }

    def << "};\n";

    return def.str();
}

bool ShaderPreprocessor::preprocess(const char* source, std::vector<std::string>& sourceStringsOut)
{
    bool isIntel = false;

    std::string vendor = g_glState->vendor(), renderer = g_glState->renderer();
    std::transform(renderer.begin(), renderer.end(), renderer.begin(), ::tolower);
    std::transform(vendor.begin(), vendor.end(), vendor.begin(), ::tolower);

    isIntel = vendor.find("intel") != std::string::npos || renderer.find("intel") != std::string::npos;

    bool uniformBufferSupport = false;
    std::string mutableSource = source;

    std::string search = "#use";
    std::size_t pos = 0;
    while ((pos = mutableSource.find(search, pos)) != std::string::npos)
    {
        auto nameStart = pos + search.length();
        while (std::isspace(mutableSource.at(nameStart)))
            ++nameStart;
        auto nameEnd = nameStart;
        while (!std::isspace(mutableSource.at(nameEnd)))
            ++nameEnd;

        std::string bufferName = mutableSource.substr(nameStart, nameEnd - nameStart);
        if (bufferName.empty() || !m_mm->paramBuffer(bufferName))
        {
                g_log->error("Shader tries to use non-existent parameter buffer %s", bufferName.c_str());
                return false;
        }
        std::string replace = buildBlockDef(m_mm->paramBuffer(bufferName));
        mutableSource.replace(pos, search.length() + bufferName.length() + 1, replace);
        pos += replace.length();

        std::size_t replacement = 0;
        while ((replacement = mutableSource.find(bufferName + ".", replacement)) != std::string::npos)
            mutableSource.replace(replacement, bufferName.length() + 1, bufferName + "_");
    }

    search = "GL_ARB_uniform_buffer_object";
    if ((pos = mutableSource.find(search, 0)) != std::string::npos)
    {
        auto newline = mutableSource.find('\n', pos);
        search = "enable";
        if (mutableSource.find(search, pos) < newline)
            uniformBufferSupport = true;
    }

    search = "#version";
    pos = 0;
    while ((pos = mutableSource.find(search, pos)) != std::string::npos)
    {
        auto verStart = pos + search.length();
        while (std::isspace(mutableSource.at(verStart)))
            ++verStart;
        auto verEnd = verStart;
        while (!std::isspace(mutableSource.at(verEnd)))
            ++verEnd;

        int version;
        std::stringstream trim;
        trim << mutableSource.substr(verStart, verEnd);
        trim.clear();
        trim >> version;
        if (version >= 140 && !isIntel)
            uniformBufferSupport = true;

        trim.str("");
        trim << "#version " << version << "\n";
        if (!uniformBufferSupport)
            trim << "#extension GL_ARB_uniform_buffer_object: enable\n";
        mutableSource.replace(pos, verEnd - pos, std::string(trim.str()));
        pos += trim.str().length();
    }

    sourceStringsOut.push_back(mutableSource);
    return true;
}

} // ns mtl
} // ns gfx
} // ns mcr
