#pragma once

#include <string>
#include <set>
#include <mcr/CoreExtern.h>
#include <mcr/NonCopyable.h>
#include <mcr/io/IFile.h>

namespace mcr {
namespace io  {

class Path
{
public:
    //! Make \c path lowercase and convert all backslashes to slashes
    MCR_CORE_EXTERN static std::string format(const char* path);

    //! Extract directory from \c path
    MCR_CORE_EXTERN static std::string dir(const char* path);

    //! Extract filename from \c path
    MCR_CORE_EXTERN static const char* filename(const char* path);

    //! Extract file extension from \c path
    MCR_CORE_EXTERN static const char* ext(const char* path);
};

class Dir
{
public:
    //! Check whether directory exists (involves some system calls)
    MCR_CORE_EXTERN static bool exists(const char* dirname);
};

class FileSystem: NonCopyable
{
public:
    //! Add directory to look for files
    MCR_CORE_EXTERN bool attachResource(const char* path);

    //! Remove previously attached directory
    MCR_CORE_EXTERN bool detachResource(const char* path);

    /*! Look for \c filename in all attached dirs and try to open it
        \param pathOut Path of a successfully opened file (if any)
        \return File handle to read from. If no file found, \c IFile::size() will return 0
    */
    MCR_CORE_EXTERN rcptr<IFile> openFile(const char* filename, std::string* pathOut = nullptr);

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

} // ns io
} // ns mcr
