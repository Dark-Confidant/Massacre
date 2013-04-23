#pragma once

#include <string>
#include <set>
#include <vector>
#include <streambuf>
#include <mcr/NonCopyable.h>
#include <mcr/Io.h>

namespace mcr {

class FileSystem;

class IFile: public RefCounted, public IReader
{
public:
    //! File size in bytes, 0 if bad
    virtual uint64 size() const = 0;

    //! Seek to absolute position
    virtual void seek(uint64 pos) = 0;

    //! Tell current absolute position
    virtual uint64 tell() const = 0;

    //! File name
    virtual const char* filename() const = 0;

    //! Pointer to \c FileSystem this file belongs to
    virtual FileSystem* fs() const = 0;
};

class FileStream: public std::streambuf, NonCopyable
{
public:
    FileStream(IFile* file, size_t bufSize = 1024, size_t putSize = 16):
        m_file(file),
        m_putSize(std::max<size_t>(putSize, 1)),
        m_buffer(std::max(bufSize, m_putSize) + m_putSize)
    {
        auto end = &m_buffer.front() + m_buffer.size();
        setg(end, end, end);
    }

protected:
    int_type underflow()
    {
        if (gptr() >= egptr())
        {
            auto base = &m_buffer.front(), start = base;

            if (base == eback())
            {
                std::memmove(base, egptr() - m_putSize, m_putSize);
                start += m_putSize;
            }

            auto bytes = m_file->read(start, m_buffer.size() - (start - base));

            if (!bytes)
                return traits_type::eof();

            setg(base, start, start + bytes);
        }

        return traits_type::to_int_type(*gptr());
    }

    rcptr<IFile> const m_file;
    size_t m_putSize;
    std::vector<char> m_buffer;
};

class Path
{
public:
    //! Make \c path lowercase and convert all backslashes to slashes
    static std::string format(const char* path);

    //! Extract directory from \c path
    static std::string dir(const char* path);

    //! Extract filename from \c path
    static const char* filename(const char* path);

    //! Extract file extension from \c path
    static const char* ext(const char* path);
};

class Dir
{
public:
    //! Check whether directory exists (involves some system calls)
    static bool exists(const char* dirname);
};

class FileSystem: NonCopyable
{
public:
    //! Add directory to look for files
    bool attachResource(const char* path);

    //! Remove previously attached directory
    bool detachResource(const char* path);

    /*! Look for \c filename in all attached dirs and try to open it
        \param pathOut Path of a successfully opened file (if any)
        \return File handle to read from. If no file found, \c IFile::size() will return 0
    */
    rcptr<IFile> openFile(const char* filename, std::string* pathOut = nullptr);

protected:
    std::set<std::string> m_paths;
};

class Resource: public RefCounted, NonCopyable
{
public:
    /*! Create Resource control and attach \c path to \c fs
        Once \c Resource is destroyed, \c path gets be detached

        \code{.cpp}
            void openStuff(FileSystem* fs)
            {
                // Find and open ../media/stuff.txt
                auto mediaRes = Resource::create(fs, "..\\Media\\"); // formatted to "../media"
                fs->openFile("stuff.txt");
                <...>
            } // <- resource is detached when it gets out of scope
        \endcode
    */
    static rcptr<Resource> create(FileSystem* fs, const char* path)
    {
        return new Resource(fs, path);
    }

    //! \c FileSystem current path is believed to be attached to
    FileSystem* fs() const { return m_fs; }

    //! Current path
    const std::string& path() const { return m_path; }

    //! Whether current path is attached to \c fs
    bool isAttached() const { return m_attached; }

protected:
    Resource(FileSystem* fs, const char* path):
        m_fs(fs), m_path(path)
    {
        m_attached = fs->attachResource(path);
    }

    virtual ~Resource()
    {
        if (m_attached)
            m_fs->detachResource(m_path.c_str());
    }

    FileSystem* m_fs;
    std::string m_path;
    bool m_attached;
};

} // ns mcr
