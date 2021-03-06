#pragma once

#include <cstring>
#include <string>
#include <mcr/RefCounted.h>

namespace mcr {
namespace io  {

class IReader: public RefCounted
{
public:
    virtual std::size_t read(void* buffer, std::size_t size) = 0;

    template            <typename T>
    std::size_t         read(T* arr, std::size_t count);

    template            <typename T, int n>
    std::size_t         read(T (&arr)[n]);

    template            <typename T>
    std::size_t         read(T& val);

    template            <typename C>
    std::size_t         readString(std::basic_string<C>& str);

    template            <typename C>
    std::size_t         readString(std::basic_string<C>& str, C term);

    template            <typename C>
    std::size_t         readString(std::basic_string<C>& str, const C* terms);

    template            <typename C>
    std::size_t         readString0(std::basic_string<C>& str);
};

} // ns io
} // ns mcr

#include "IReader.inl"
