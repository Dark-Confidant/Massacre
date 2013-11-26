#include "Universe.h"
#include "ShaderPreprocessor.h"

#include <sstream>
#include <mcr/Log.h>
#include <mcr/gfx/mtl/Manager.h>

namespace mcr    {
namespace detail {

class LogWrapper
{
public:
    enum FlushType { Flush };

    LogWrapper& operator<<(FlushType)
    {
        g_log->error("%s", m_stream.str().c_str());
        m_stream.str("");

        return *this;
    }

    template <typename T>
    LogWrapper& operator<<(const T& thingy)
    {
        m_stream << thingy;
        return *this;
    }

private:
    std::stringstream m_stream;
};

} // ns detail
} // ns mcr


//////////////////////////////////////////////////////////////////////////
// Directives

namespace directives {

struct Version
{
    int version;
    std::string profile;

    Version(): version(0) {}
};

struct Extension
{
    enum Behavior
    {
        Enable,
        Require,
        Warn,
        Disable
    };

    std::string extName;
    Behavior behavior;

    bool allowed() const
    {
        return behavior == Enable || behavior == Require;
    }
};

struct Use
{
    std::string bufferName;
};

} // ns directives

//////////////////////////////////////////////////////////////////////////
// Preprocessing, naturally

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

    static mcr::detail::LogWrapper errlog;

    std::string mutableSource = source;

    auto it  = mutableSource.c_str();
    auto end = it + mutableSource.size();

    sourceStringsOut.push_back(source);
    std::string search = "#use";
    std::string replace = buildBlockDef(m_mm->paramBuffer("Camera"));
    size_t pos = 0;
    while ((pos = mutableSource.find(search, pos)) != std::string::npos) {
	    mutableSource.replace(pos, search.length(), replace);
	    pos += replace.length();
	    std::cout << "Pos: " << pos << std::endl;
    };

    std::cout << "Before: " << std::endl << source << std::endl;
    std::cout << "After: " << std::endl << mutableSource << std::endl;

    std::abort();
    return true;
}

} // ns mtl
} // ns gfx
} // ns mcr
