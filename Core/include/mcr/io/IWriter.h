#pragma once

#include <cstddef>
#include <cstring>
#include <string>

namespace mcr {
namespace io  {

class IWriter
{
public:
    virtual ~IWriter() {}

    virtual std::size_t write(const void* buffer, std::size_t size) = 0;

    template            <typename T>
    std::size_t         write(const T* arr, std::size_t count);

    template            <typename T, int n>
    std::size_t         write(const T (&arr)[n]);

    template            <typename T>
    std::size_t         write(const T& val);

    template            <typename C>
    std::size_t         writeString(std::basic_string<C>& str);

    template            <typename C>
    std::size_t         writeString0(std::basic_string<C>& str);
    std::size_t         writeString0(const char* str);
    std::size_t         writeString0(const wchar_t* str);
};

} // ns io
} // ns mcr

#include "IWriter.inl"
