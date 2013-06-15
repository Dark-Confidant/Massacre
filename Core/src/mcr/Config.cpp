#include <mcr/Config.h>

#include <cstdio>
#include <mcr/Log.h>
#include <mcr/io/LineParser.h>

namespace mcr {

bool Config::load(io::IReader* stream, bool report)
{
    if (!stream)
    {
        if (report)
            g_log->warn("No config stream");

        return false;
    }

    for (io::LineParser parser(stream); parser.readLine();)
    {
        char key[128], value[128];
        if (std::sscanf(parser.line().c_str(), "%127[^=: \xA0\t] %*1[=:] %127s", key, value) != 2)
        {
            if (report)
                g_log->warn("Config syntax error: %s", parser.line().c_str());

            return false;
        }

        m_vars[key] = value;

        if (report)
            g_log->debug("var %s = %s", key, value);
    }

    return true;
}

} // ns mcr
