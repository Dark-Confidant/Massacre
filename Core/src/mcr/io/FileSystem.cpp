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
#include <boost/foreach.hpp>

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
// Resource management

bool FileSystem::attachResource(const char* path)
{
    auto neatPath = Path::format(path);
    return Dir::exists(neatPath.c_str()) && m_paths.insert(neatPath).second;
}

bool FileSystem::detachResource(const char* path)
{
    return m_paths.erase(Path::format(path)) != 0;
}


//////////////////////////////////////////////////////////////////////////
// Standard iostream-based file reader

namespace
{
    class StdStreamFile: public IFile
    {
    public:
        using RefCounted::operator new;
        using RefCounted::operator delete;

        StdStreamFile(FileSystem* fs, const char* filename):
            m_fs(fs),
            m_filename(filename),
            m_stream(filename, std::ios::binary),
            m_size(0u)
        {
            if (m_stream.good())
            {
                m_stream.seekg(0, std::ios::end);
                m_size = m_stream.tellg();
                m_stream.seekg(0, std::ios::beg);
            }
        }

        size_t read(void* buffer, size_t size)
        {
            m_stream.read(static_cast<char*>(buffer), size);
            return (size_t) m_stream.gcount();
        }

        uint64 size() const
        {
            return m_size;
        }

        void seek(uint64 pos)
        {
            m_stream.seekg(pos);
        }

        uint64 tell() const
        {
            return m_stream.tellg();
        }

        const char* filename() const
        {
            return m_filename.c_str();
        }

        FileSystem* fs() const
        {
            return m_fs;
        }

    protected:
        FileSystem* m_fs;
        std::string m_filename;
        mutable std::ifstream m_stream;
        uint64 m_size;
    };
}


//////////////////////////////////////////////////////////////////////////
// File search & access

rcptr<IFile> FileSystem::openFile(const char* filename, std::string* pathOut)
{
    rcptr<StdStreamFile> file = new StdStreamFile(this, filename);

    if (file->size())
    {
        if (pathOut)
            *pathOut = filename;

        return file;
    }

    BOOST_FOREACH (auto& path, m_paths)
    {
        auto guess = path + '/' + filename;

        file = new StdStreamFile(this, guess.c_str());

        if (file->size())
        {
            if (pathOut)
                *pathOut = guess;

            break;
        }
    }

    return file;
}

} // ns io
} // ns mcr
