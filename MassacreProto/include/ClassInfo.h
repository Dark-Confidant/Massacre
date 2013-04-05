#pragma once

#include <typeinfo>
#include <string>
#include <cstdlib>

namespace mcr    {
namespace detail {

struct ClassInfo
{
    static std::string truncName(const char* name)
    {
#if defined(_MSC_VER)

        while (*name && *name != ' ')
            ++name;

        return name;

#elif defined(__GNUC__)

        char* end;
        auto len = strtoul(name, &end, 10);

        return std::string(end, (std::string::size_type) len);

#else
#   error Compiler not supported
#endif
    }

    //! DJB2a
    static unsigned hashName(const char* name)
    {
        unsigned hash = 5381;

        while (int c = *name++)
            hash = ((hash << 5) + hash) ^ c;

        return hash;
    }
};

} // namespace detail

template <class T>
class ClassInfo
{
public:
    static const std::string& name()
    {
        static std::string name = detail::ClassInfo::truncName(typeid(T).name());
        return name;
    }

    static unsigned hash()
    {
        static unsigned hash = detail::ClassInfo::hashName(ClassInfo<T>::name);
        return hash;
    }
};

} // namespace mcr
