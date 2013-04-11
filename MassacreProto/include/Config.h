#pragma once

#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/any.hpp>

#include "FileSystem.h"
#include "Debug.h"

namespace mcr    {
namespace detail {

class ConfigVar
{
public:
    ConfigVar(const std::string* str): m_valueString(str) {}
    ~ConfigVar() {} // inherit with care

    template <class T>
    operator T() const
    {
        if (m_valueString)
            return boost::lexical_cast<T>(*m_valueString);

        return T();
    }

protected:
    const std::string* const m_valueString;
};

class ConfigVarWithDefault: public ConfigVar
{
public:
    template <typename T>
    ConfigVarWithDefault(const ConfigVar& var, const T& defaultValue):
    ConfigVar(var),
        m_defaultValue(defaultValue) {}

    ~ConfigVarWithDefault() {}


    template <typename T>
    operator T() const
    {
        if (!m_valueString)
            return boost::any_cast<T>(m_defaultValue);

        return static_cast<const ConfigVar&>(*this);
    }

protected:
    boost::any m_defaultValue;
};

template <typename T>
inline ConfigVarWithDefault operator||(const ConfigVar& var, const T& defaultValue)
{
    return ConfigVarWithDefault(var, defaultValue);
}

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
