namespace mcr {
namespace io  {

template <typename T>
inline std::size_t IReader::read(T* arr, std::size_t count)
{
    return read(static_cast<void*>(arr), count * sizeof(T));
}

template <typename T, int n>
inline std::size_t IReader::read(T (&arr)[n])
{
    return read<T>(arr, n);
}

template <typename T>
inline std::size_t IReader::read(T& val)
{
    return read(&val, 1);
}

template <typename C>
inline std::size_t IReader::readString(std::basic_string<C>& str)
{
    std::size_t len, bytes;

    bytes = read(len);
    str.resize(len);

    return bytes + read(&str[0], len);
}

template <typename C>
inline std::size_t IReader::readString(std::basic_string<C>& str, C term)
{
    std::size_t bytes, total = 0;

    for (C chr; bytes = read(chr), bytes && chr != term;)
    {
        total += bytes;
        str += chr;
    }

    return total;
}

template <typename C>
inline std::size_t IReader::readString0(std::basic_string<C>& str)
{
    return readString(str, C(0));
}

} // ns io
} // ns mcr
