#pragma once

#include <map>
#include <sstream>
#include <mcr/io/IFile.h>

namespace mcr {

class Config
{
public:
    Config() {}
    virtual ~Config() {}

    MCR_CORE_EXTERN bool load(io::IFile* file, bool report = true);

    template <typename T>
    bool query(const std::string& varName, T& varOut, const T& varDefault = T()) const
    {
        auto it = m_vars.find(varName);
        if (it != m_vars.end())
        {
            std::stringstream ss;

            ss << it->second;
            ss >> varOut;

            return true;
        }

        varOut = varDefault;
        return false;
    }

private:
    std::map<std::string, std::string> m_vars;
};

} // ns mcr
