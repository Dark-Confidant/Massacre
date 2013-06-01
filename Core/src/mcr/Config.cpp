#include <mcr/Config.h>

#include <cctype>
#include <cstdio>
#include <algorithm>
#include <mcr/Debug.h>

namespace mcr {

bool Config::load(io::IReader* stream, bool report)
{
    if (!stream)
    {
        if (report)
            debug("No config");

        return false;
    }

    for (std::string line; stream->readString(line, "\r\n");)
    {
        if (std::all_of(line.begin(), line.end(), ::isspace))
            continue;

        char key[128], value[128];
        if (std::sscanf(line.c_str(), "%127[^=: \xA0] %*[=:] %127s", key, value) != 2)
        {
            if (report)
                debug("Config syntax error: %s", line.c_str());

            return false;
        }

        m_vars[key] = value;

        if (report)
            debug("var %s = %s", key, value);
    }

    return true;
}

} // ns mcr
