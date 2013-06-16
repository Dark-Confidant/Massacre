#include <mcr/io/FileSystem.h>

#if defined(MCR_PLATFORM_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <Windows.h>
#elif defined(MCR_PLATFORM_LINUX)
#   include <sys/stat.h>
#   include <dirent.h>
#endif

#include <algorithm>
#include <fstream>

namespace mcr {
namespace io  {

//////////////////////////////////////////////////////////////////////////
// Path functions

namespace {

char toPathChar(char c)
{ return c == '\\' ? '/' : c; }

bool isSlash(char c)
{ return c == '/' || c == '\\'; }

bool isDot(char c)
{ return c == '.'; }

template <typename F>
const char* lastPartFrom(const char* path, F pred)
{
    const char* result = path;

    for (; *path; ++path)
        if (pred(*path))
            result = path + 1;

    return result;
}

} // ns

std::string Path::format(const char* path)
{
    std::string result = path;
    std::transform(result.cbegin(), result.cend(), result.begin(), toPathChar);

    if (result.back() == '/')
        result.erase(result.size() - 1);

    return result;
}

std::string Path::dir(const char* path)
{
    std::string dir = path;

    auto slash = dir.find_last_of("\\/");
    return slash != std::string::npos ? dir.substr(0, slash) : ".";
}

const char* Path::filename(const char* path)
{
    return lastPartFrom(path, isSlash);
}

const char* Path::ext(const char* path)
{
    return lastPartFrom(path, isDot);
}


//////////////////////////////////////////////////////////////////////////
// Dir functions

bool Dir::exists(const char* dirname)
{
#if defined(MCR_PLATFORM_WINDOWS)

    auto fattrs = GetFileAttributesA(dirname);
    return fattrs != INVALID_FILE_ATTRIBUTES && (fattrs & FILE_ATTRIBUTE_DIRECTORY) != 0;

#elif defined(MCR_PLATFORM_LINUX)

    struct stat st;
    return stat(dirname, &st) == 0 && S_ISDIR(st.st_mode);

#endif
}


//////////////////////////////////////////////////////////////////////////
// Root

bool FileSystem::setRoot(const char* dir, std::string* oldRootOut)
{
    std::string newRoot = dir;
    std::transform(newRoot.begin(), newRoot.end(), newRoot.begin(), toPathChar);

    if (newRoot.back() != '/')
        newRoot += '/';

    if (!Dir::exists(newRoot.c_str()))
        return false;

    if (oldRootOut)
        *oldRootOut = m_root;

    m_root = newRoot;
    return true;
}


//////////////////////////////////////////////////////////////////////////
// Standard iostream-based reader and writer

namespace {
class StdFstreamReader: public IFileReader
{
public:
    using RefCounted::operator new;
    using RefCounted::operator delete;

    StdFstreamReader(const char* filename, bool binary):
        m_filename(filename),
        m_stream(filename, binary ? std::ios::binary : std::ios::in),
        m_size(0u)
    {
        if (m_stream.good())
        {
            m_stream.seekg(0, std::ios::end);
            m_size = m_stream.tellg();
            m_stream.seekg(0, std::ios::beg);
        }
    }

    std::size_t read(void* buffer, std::size_t size)
    {
        m_stream.read(static_cast<char*>(buffer), size);
        return (std::size_t) m_stream.gcount();
    }

    uint64 size() const
    {
        return m_size;
    }

    uint64 tell() const
    {
        return m_stream.tellg();
    }

    void seek(uint64 pos)
    {
        m_stream.seekg(pos);
    }

    const char* filename() const
    {
        return m_filename.c_str();
    }

    bool good() const
    {
        return m_stream.good();
    }

protected:
    std::string m_filename;
    mutable std::ifstream m_stream;
    uint64 m_size;
};

class StdFstreamWriter: public IFileWriter
{
public:
    using RefCounted::operator new;
    using RefCounted::operator delete;

    StdFstreamWriter(const char* filename, bool binary):
        m_filename(filename),
        m_stream(filename, binary ? std::ios::binary | std::ios::trunc : std::ios::trunc) {}

    std::size_t write(const void* buffer, std::size_t size)
    {
        m_stream.write(static_cast<const char*>(buffer), size);
        return m_stream.bad() ? 0u : size;
    }

    uint64 tell() const
    {
        return m_stream.tellp();
    }

    void seek(uint64 pos)
    {
        m_stream.seekp(pos);
    }

    const char* filename() const
    {
        return m_filename.c_str();
    }

    bool good() const
    {
        return m_stream.good();
    }

protected:
    std::string m_filename;
    mutable std::ofstream m_stream;
};
} // ns


//////////////////////////////////////////////////////////////////////////
// File search & access

rcptr<IFileReader> FileSystem::openReader(const char* filename, bool binary)
{
    rcptr<StdFstreamReader> file = new StdFstreamReader((m_root + filename).c_str(), binary);

    if (!file->good())
        return nullptr;

    return file;
}

rcptr<IFileWriter> FileSystem::openWriter(const char* filename, bool binary)
{
    rcptr<StdFstreamWriter> file = new StdFstreamWriter((m_root + filename).c_str(), binary);

    if (!file->good())
        return nullptr;

    return file;
}

} // ns io
} // ns mcr
