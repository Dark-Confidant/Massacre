#include "Universe.h"
#include <mcr/PluginManager.h>

#ifdef MCR_PLATFORM_LINUX
#   include <dlfcn.h>
#endif

#include <boost/filesystem.hpp>

namespace mcr {

const IPlugin* PluginManager::loadPlugin(const char* path)
{
    namespace fs = boost::filesystem;

    fs::path wpath = path;
    (void) path;

    boost::system::error_code err;
    if (!fs::is_regular_file(wpath, err))
    {
#if defined(MCR_PLATFORM_WINDOWS)
        wpath.replace_extension(".dll");
#elif defined(MCR_PLATFORM_LINUX)
        wpath.replace_extension(".so");
#endif
        if (!fs::is_regular_file(wpath, err))
            return nullptr;
    }

#if defined(MCR_PLATFORM_WINDOWS)

    auto module = LoadLibraryW(wpath.c_str());
    if (module == NULL)
        return nullptr;

    typedef const IPlugin&(__cdecl *PluginEntry)();

    auto entry = PluginEntry(GetProcAddress(module, "plugin"));
    if (!entry)
    {
        FreeLibrary(module);
        return nullptr;
    }

    return &entry();

#elif defined(MCR_PLATFORM_LINUX)

    auto dso = dlopen(wpath.c_str(), RTLD_NOW);
    if (!dso)
        return nullptr;

    typedef const IPlugin&(*PluginEntry)();

    auto entry = PluginEntry(dlsym(dso, "plugin"));
    if (!entry)
    {
        dlclose(dso);
        return nullptr;
    }

    return &entry();

#endif
    return nullptr;
}

uint PluginManager::loadPlugins(const char* dir)
{
    using namespace boost::filesystem;
    using boost::system::error_code;

    path dirp = dir;
    uint loaded = 0;
    error_code err;

    if (!exists(dirp, err) || !is_directory(dirp, err))
        return loaded;

    for (directory_iterator it(dirp, err), end; it != end; ++it)
    {
        auto plugin = loadPlugin(it->path().string().c_str());
        if (!plugin)
            continue;

        for (uint i = 0, n = plugin->numImageImporters(); i < n; ++i)
            m_imageImporters.push_back(plugin->createImageImporter(i));

        for (uint i = 0, n = plugin->numGeometryImporters(); i < n; ++i)
            m_meshImporters.push_back(plugin->createGeometryImporter(i));

        for (uint i = 0, n = plugin->numGeometryExporters(); i < n; ++i)
            m_meshExporters.push_back(plugin->createGeometryExporter(i));

        ++loaded;
    }

    return loaded;
}

} // ns mcr
