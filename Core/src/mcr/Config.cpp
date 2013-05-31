#include <mcr/Config.h>

#include <cctype>
#include <cstdio>
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

    std::string buffer;
    stream->readString0(buffer);

    for (auto it = buffer.c_str(), end = it + buffer.size(); it != end;)
    {
        char key[32], value[32];
        int len = 0;

        if (std::sscanf(it, "%[^=: \xA0] %*[=:] %[^\n\r]%n", key, value, &len) == 2)
            it += len;
        else
        {
            if (report)
            {
                char line[128];
                std::sscanf(it, "%[^\n\r]", line);
                debug("Config syntax error: %s", line);
            }
            return false;
        }

        m_vars[key] = value;

        if (report)
            debug("var %s = %s", key, value);

        while (std::isspace(*it))
            ++it;
    }

    return true;
}

} // ns mcr
