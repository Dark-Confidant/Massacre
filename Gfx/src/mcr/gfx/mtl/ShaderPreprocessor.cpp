#include "Universe.h"
#include "ShaderPreprocessor.h"

#include <sstream>
#include <cstring>
#include "mcr/gfx/GLState.h"
#include <mcr/Log.h>
#include <mcr/gfx/mtl/Manager.h>

#ifdef MCR_PLATFORM_WINDOWS
#define strcasestr lstrcmpi
#endif

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
    bool isIntel = false;

    if ((strcasestr(g_glState->renderer().c_str(), "intel") != NULL) ||
	(strcasestr(g_glState->vendor().c_str(), "intel") != NULL))
	    isIntel = true;
	
    bool uniformBufferSupport = false;
    std::string mutableSource = source;

    std::string search = "#use ";
    std::size_t pos = 0;
    while ((pos = mutableSource.find(search, pos)) != std::string::npos)
    {
	    auto nameStart = pos + search.length();
	    auto nameEnd = nameStart;
	    while (mutableSource.at(nameEnd) != '\n')
		    ++nameEnd;
	    std::string buffer_name = mutableSource.substr(nameStart,
							   nameEnd - nameStart);
	    std::string replace = buildBlockDef(m_mm->paramBuffer(buffer_name));
	    mutableSource.replace(pos, search.length() + buffer_name.length(), replace);
	    pos += replace.length();
	    size_t replacement = 0;
	    while ((replacement =
		    mutableSource.find(buffer_name + ".", replacement)) != std::string::npos)
		mutableSource.replace(replacement, buffer_name.length() + 1, buffer_name + "_");
    };

    search = "GL_ARB_uniform_buffer_object";
    if ((pos = mutableSource.find(search, 0)) != std::string::npos)
    {
	    auto newline = mutableSource.find('\n', pos);
	    search = "enable";
	    if (mutableSource.find(search, pos) < newline)
	    {
		    uniformBufferSupport = true;
		    g_log->debug("uniform_buffer_support explicitly enabled");
	    }
    }

    search = "#version";
    pos = 0;
    while ((pos = mutableSource.find(search, pos)) != std::string::npos)
    {
	    auto newline = mutableSource.find('\n', pos);
	    int version;
	    std::stringstream trim;
	    trim << mutableSource.substr(pos + search.length(), newline);
	    trim.clear();
	    trim >> version;
	    version = std::max(version, 140);
	    if (isIntel)
		    version = 130;
	    if (version >= 140)
		    uniformBufferSupport = true;

	    trim.str("");
	    trim << "#version " << version << "\n";
	    if (!uniformBufferSupport)
		    trim << "#extension GL_ARB_uniform_buffer_object: enable\n";
	    mutableSource.replace(pos, newline - pos, std::string(trim.str()));
	    pos += trim.str().length();
    }

    sourceStringsOut.push_back(mutableSource);
    return true;
}

} // ns mtl
} // ns gfx
} // ns mcr
