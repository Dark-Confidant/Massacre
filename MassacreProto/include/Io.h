#pragma once

#include <cstring>
#include "RefCounted.h"

namespace mcr {

class IReader
{
public:
    virtual size_t read(void* buffer, size_t size) = 0;


    template <typename T>
    size_t read(T* arr, size_t count)
    {
        return read(static_cast<void*>(arr), count * sizeof(T));
    }

    template <typename T, int n>
    size_t read(T (&arr)[n])
    {
        return read<T>(arr, n);
    }

    template <typename T>
    size_t read(T& val)
    {
        return read(&val, 1);
    }

    template <typename C>
    size_t readString(std::basic_string<C>& str)
    {
        size_t len, bytes;

        bytes = read(len);
        str.resize(len);

        return bytes + read(&str[0], len);
    }

    template <typename C>
    size_t readString(std::basic_string<C>& str, C term)
    {
        size_t bytes, total = 0;

        for (C chr; bytes = read(chr), bytes && chr != term;)
        {
            total += bytes;
            str += chr;
        }

        return total;
    }

    template <typename C>
    size_t readString0(std::basic_string<C>& str)
    {
        return readString(str, C(0));
    }
};

class IWriter
{
public:
    virtual size_t write(const void* buffer, size_t size) = 0;


    template <typename T>
    size_t write(const T* arr, size_t count)
    {
        return write(static_cast<const void*>(arr), count * sizeof(T));
    }

    template <typename T, int n>
    size_t write(const T (&arr)[n])
    {
        return write<T>(arr, n);
    }

    template <typename T>
    size_t write(const T& val)
    {
        return write(&val, 1);
    }

    template <typename C>
    size_t writeString0(std::basic_string<C>& str)
    {
        return write(str.c_str(), str.size() + 1);
    }

    size_t writeString0(const char* str)
    {
        return write(str, std::strlen(str) + 1);
    }

    size_t writeString0(const wchar_t* str)
    {
        return write(str, std::wcslen(str) + 1);
    }

    template <typename C>
    size_t writeString(std::basic_string<C>& str)
    {
        size_t bytes = write(str.size());
        return bytes + write(str.c_str(), str.size());
    }
};

} // ns mcr
