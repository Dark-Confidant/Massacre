#include "Universe.h"
#include "ShaderPreprocessor.h"

#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_auto.hpp>
#include <boost/spirit/include/qi_raw.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/qi_no_skip.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <mcr/Log.h>
#include <mcr/gfx/mtl/Manager.h>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

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

typedef boost::variant
<
    Version,
    Extension,
    Use
>
Any;

struct Directive
{
    const char* first;
    Any directive;
    const char* last;
};

} // ns directives

BOOST_FUSION_ADAPT_STRUCT(
    directives::Version,
    (int, version)
    (std::string, profile));

BOOST_FUSION_ADAPT_STRUCT(
    directives::Extension,
    (std::string, extName)
    (directives::Extension::Behavior, behavior));

BOOST_FUSION_ADAPT_STRUCT(
    directives::Use,
    (std::string, bufferName));

BOOST_FUSION_ADAPT_STRUCT(
    directives::Directive,
    (const char*, first)
    (directives::Any, directive)
    (const char*, last));


//////////////////////////////////////////////////////////////////////////
// Grammar

namespace {

template <typename It, typename Skipper = qi::unused_type>
struct Grammar: public qi::grammar<It, std::vector<directives::Directive>(), Skipper>
{

    qi::rule<It, It()>          pos;
    qi::rule<It>                eol;
    qi::rule<It>                waitHash;
    qi::rule<It, std::string()> bufferName;
    qi::rule<It, std::string()> extName;

    qi::symbols<char, directives::Extension::Behavior> extBehavior;

    qi::rule<It, directives::Version(),   Skipper> version;
    qi::rule<It, directives::Extension(), Skipper> extension;
    qi::rule<It, directives::Use(),       Skipper> use;
    qi::rule<It, directives::Directive(), Skipper> directive;

    qi::rule<It, std::vector<directives::Directive>(), Skipper> preprocess;


    Grammar(mcr::detail::LogWrapper& errlog):
    Grammar::base_type(preprocess, "preprocess"),
        bufferName  (std::string("quoted or plain name")),
        version     (std::string("#version")),
        use         (std::string("#use")),
        directive   (std::string("directive"))
    {
        using boost::spirit::_1;
        using boost::spirit::_2;
        using boost::spirit::_3;
        using boost::spirit::_4;

        pos      = qi::raw[qi::eps][qi::_val = phx::begin(_1)];
        eol      = *(qi::char_ - qi::eol) > qi::eol;
        waitHash = *(qi::char_ - '#');

        bufferName =
                ('"' > +(qi::char_ - '"') > '"')
            |   ('<' > +(qi::char_ - '>') > '>')
            |   +(qi::char_ - qi::space)
            ;

        extName = +(qi::char_ - qi::space - ':');

        extBehavior.add
            ("enable",  directives::Extension::Enable)
            ("require", directives::Extension::Require)
            ("warn",    directives::Extension::Warn)
            ("disable", directives::Extension::Disable);

        version    = "version" > qi::int_ > -+qi::char_("a-z");
        use        = "use" > bufferName;
        extension  = "extension" > extName > ':' > extBehavior;
        directive  = pos >> '#' >> -(version | extension | use) >> qi::no_skip[eol >> pos];

        preprocess = *(waitHash >> directive);

        qi::on_error<qi::fail>
        (
            preprocess,
            phx::ref(errlog)
                << "Syntax error: expecting " << _4 << std::endl
                << ",--" << std::endl
                << phx::construct<std::string>(_3, _2)
                << mcr::detail::LogWrapper::Flush
        );
    }
};

} // ns


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
    namespace charset = boost::spirit::standard;

    static mcr::detail::LogWrapper errlog;
    static Grammar<const char*, charset::space_type> grammar(errlog);

    std::string mutableSource = source;

    auto it  = mutableSource.c_str();
    auto end = it + mutableSource.size();

    std::vector<directives::Directive> directives;

    if (!qi::phrase_parse(it, end, grammar, charset::space, directives))
        return false;

    if (directives.empty())
    {
        sourceStringsOut.push_back(source);
        return true;
    }

    bool usesBuffers = false, supportsBuffers = false;
    auto
        first = mutableSource.c_str(),
        last  = mutableSource.c_str() + mutableSource.size();

    BOOST_FOREACH (auto& def, directives)
    {
        if (auto version = boost::get<directives::Version>(&def.directive))
        {
            supportsBuffers = supportsBuffers || version->version >= 140;

            if (first != def.first)
                sourceStringsOut.push_back(std::string(first, def.first));

            sourceStringsOut.push_back(std::string(def.first, def.last));

            first = last = def.last;
        }
        if (auto ext = boost::get<directives::Extension>(&def.directive))
        {
            supportsBuffers = supportsBuffers
                || (ext->extName == "GL_ARB_uniform_buffer_object" && ext->allowed());

            last = def.last;
        }
        else if (auto use = boost::get<directives::Use>(&def.directive))
        {
            usesBuffers = true;

            if (first != def.first)
                sourceStringsOut.push_back(std::string(first, def.first));

            if (auto buffer = m_mm->paramBuffer(use->bufferName))
            {
                sourceStringsOut.push_back(buildBlockDef(buffer));
                boost::replace_all(mutableSource, buffer->name() + '.', buffer->name() + '_');
            }

            first = last = def.last;
        }
        else
        {
            // ignore
            last = def.last;
        }
    }

    if (usesBuffers && !supportsBuffers)
    {
        if (sourceStringsOut.empty())
            sourceStringsOut.push_back("#version 130\n");

        sourceStringsOut[0] += "#extension GL_ARB_uniform_buffer_object: enable\n";
    }

    if (first != end)
        sourceStringsOut.push_back(std::string(first, end));

    return true;
}

} // ns mtl
} // ns gfx
} // ns mcr
