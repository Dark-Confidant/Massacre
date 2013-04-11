#pragma once

#include <map>
#include <boost/lexical_cast.hpp>

#include "FileSystem.h"
#include "Debug.h"

namespace mcr    {
namespace detail {

class ConfigVar
{
public:
    ConfigVar(const std::string* str): m_valueString(str) {}
    ~ConfigVar() {} // not intended to be inherited

    template <class T>
    operator T() const
    {
        if (m_valueString)
            return boost::lexical_cast<T>(*m_valueString);

        return T();
    }

private:
    const std::string* const m_valueString;
};

} // ns detail


class Config
{
public:
    Config() {}
    virtual ~Config() {}

    void load(IFile* file);

    detail::ConfigVar operator[](const std::string& varName) const
    {
        auto it = s_configuration.find(varName);
        if (it != s_configuration.end())
            return &it->second;

        debug("Missing config variable `%s`", varName.c_str());
        return nullptr;
    }

private:
    static std::map<std::string, std::string> s_configuration;
};

} // ns mcr
