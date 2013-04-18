#pragma once

#include <string>
#include <type_traits>
#include "math/Matrix.h"

namespace mcr {

template <typename T>
int glType();

struct GlslType
{
    int glTypeEnum;
    int length;
};

namespace detail {

template <typename T>
size_t glslArithValueLength();

template <> inline size_t glslArithValueLength<float> () { return 8;  }
template <> inline size_t glslArithValueLength<double>() { return 14; }
template <> inline size_t glslArithValueLength<int>   () { return 11; }
template <> inline size_t glslArithValueLength<uint>  () { return 10; }
template <> inline size_t glslArithValueLength<bool>  () { return 5;  }

template <typename T>
const char* glslArithValueSprintfLiteral();

template <> inline const char* glslArithValueSprintfLiteral<float> () { return "%.6f";  }
template <> inline const char* glslArithValueSprintfLiteral<double>() { return "%.12f"; }
template <> inline const char* glslArithValueSprintfLiteral<int>   () { return "%d";    }
template <> inline const char* glslArithValueSprintfLiteral<uint>  () { return "%u";    }
template <> inline const char* glslArithValueSprintfLiteral<bool>  () { return "%d";    }

template <typename T> const char* glslTypePrefix();

template <> inline const char* glslTypePrefix<float> () { return "";  }
template <> inline const char* glslTypePrefix<double>() { return "d"; }
template <> inline const char* glslTypePrefix<int>   () { return "i"; }
template <> inline const char* glslTypePrefix<uint>  () { return "u"; }
template <> inline const char* glslTypePrefix<bool>  () { return "b"; }

template <typename T> inline size_t glslTypePrefixLength       () { return 1; }
template <>           inline size_t glslTypePrefixLength<float>() { return 0; }

template <int n> const char* glslTypePostfix();

template <> inline const char* glslTypePostfix<1> () { return "";     }
template <> inline const char* glslTypePostfix<2> () { return "vec2"; }
template <> inline const char* glslTypePostfix<3> () { return "vec3"; }
template <> inline const char* glslTypePostfix<4> () { return "vec4"; }
template <> inline const char* glslTypePostfix<16>() { return "mat4"; }

template <int n> inline size_t glslTypePostfixLength   () { return 4; }
template <>      inline size_t glslTypePostfixLength<1>() { return 0; }

template <int n, typename T>
inline std::string glslArithArrayToString(const T* arr, GlslType* typeSpec)
{
    if (typeSpec)
    {
        typeSpec->glTypeEnum = glType<T>();
        typeSpec->length     = n;
    }

    std::string buf(n * (glslArithValueLength<T>() + 2) + 7, '\0'); // + for type and commas
    char* str = &buf[0];

    if (n != 1)
    {
        strncpy(str, glslTypePrefix<T>(), glslTypePrefixLength<T>());
        str += glslTypePrefixLength<T>();

        strncpy(str, glslTypePostfix<n>(), glslTypePostfixLength<n>());
        str += glslTypePostfixLength<n>();

        *str++ = '(';
    }

    str += sprintf(str, glslArithValueSprintfLiteral<T>(), arr[0]);

    for (int i = 1; i != n; ++i)
    {
        str[0] = ',';
        str[1] = ' ';
        str += 2 + sprintf(str + 2, glslArithValueSprintfLiteral<T>(), arr[i]);
    }

    if (n != 1)
        *str++ = ')';

    buf.erase(str - buf.c_str());
    return buf;
}

} // ns detail

template <typename T, int n>
std::string glslValueString(const math::Vector<T, n>& v, GlslType* typeSpec = nullptr)
{
    return detail::glslArithArrayToString<n>(static_cast<const T*>(v), typeSpec);
}

template <typename T>
std::string glslValueString(const math::Matrix4x4<T>& m, GlslType* typeSpec = nullptr)
{
    return detail::glslArithArrayToString<16>(static_cast<const T*>(m), typeSpec);
}

template <typename T>
std::string glslValueString(T val, GlslType* typeSpec = nullptr)
{
    return detail::glslArithArrayToString<1>(&val, typeSpec);
}


template <typename T>
inline T glslClamp(const T& value, double minv, double maxv,
                   typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr)
{
    return math::clamp(value, (T) minv, (T) maxv);
}

template <typename T>
inline const T& glslClamp(const T& value, double, double,
                          typename std::enable_if<!std::is_arithmetic<T>::value>::type* = nullptr)
{
    return value;
}

} // ns mcr
