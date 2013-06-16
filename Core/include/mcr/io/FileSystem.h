#pragma once

#include <string>
#include <mcr/CoreExtern.h>
#include <mcr/NonCopyable.h>
#include <mcr/io/IFileReader.h>
#include <mcr/io/IFileWriter.h>

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
    FileSystem(): m_root("./") {}
    ~FileSystem() {} // inherit not

    const std::string&      root() const;
    MCR_CORE_EXTERN bool    setRoot(const char* dir, std::string* oldRootOut = nullptr);

    MCR_CORE_EXTERN rcptr<IFileReader>  openReader(const char* filename, bool binary = true);
    MCR_CORE_EXTERN rcptr<IFileWriter>  openWriter(const char* filename, bool binary = true);

private:
    std::string m_root;
};

} // ns io
} // ns mcr

#include "FileSystem.inl"
