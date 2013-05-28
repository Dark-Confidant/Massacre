namespace mcr {
namespace io  {

template <typename T>
inline std::size_t IWriter::write(const T* arr, std::size_t count)
{
    return write(static_cast<const void*>(arr), count * sizeof(T));
}

template <typename T, int n>
inline std::size_t IWriter::write(const T (&arr)[n])
{
    return write<T>(arr, n);
}

template <typename T>
inline std::size_t IWriter::write(const T& val)
{
    return write(&val, 1);
}

template <typename C>
inline std::size_t IWriter::writeString(std::basic_string<C>& str)
{
    std::size_t bytes = write(str.size());
    return bytes + write(str.c_str(), str.size());
}

template <typename C>
inline std::size_t IWriter::writeString0(std::basic_string<C>& str)
{
    return write(str.c_str(), str.size() + 1);
}

inline std::size_t IWriter::writeString0(const char* str)
{
    return write(str, std::strlen(str) + 1);
}

inline std::size_t IWriter::writeString0(const wchar_t* str)
{
    return write(str, std::wcslen(str) + 1);
}

} // ns io
} // ns mcr
