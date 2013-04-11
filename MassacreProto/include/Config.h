#pragma once

#include <map>
#include <sstream>

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
        {
            std::stringstream ss;
            ss << *m_valueString;

            T result;
            ss >> result;

            return result;
        }
        return T();
    }

protected:
    const std::string* const m_valueString;
};

template <typename T>
class ConfigVarWithDefault: public ConfigVar
{
public:
    ConfigVarWithDefault(const ConfigVar& var, const T& defaultValue):
    ConfigVar(var),
        m_defaultValue(defaultValue) {}

    ~ConfigVarWithDefault() {}


    operator T() const
    {
        if (!m_valueString)
            return m_defaultValue;

        return static_cast<const ConfigVar&>(*this);
    }

protected:
    T m_defaultValue;
};

template <typename T>
inline ConfigVarWithDefault<T> operator||(const ConfigVar& var, const T& defaultValue)
{
    return ConfigVarWithDefault<T>(var, defaultValue);
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
