#pragma once

#include <vector>
#include <mcr/GfxExtern.h>
#include <mcr/IPlugin.h>

namespace mcr {

class PluginManager
{
public:
    MCR_GFX_EXTERN  static const IPlugin* loadPlugin(const char* path);

    MCR_GFX_EXTERN uint loadPlugins(const char* dir = ".");
    
private:
    std::vector<rcptr<gfx::mtl::IImageImporter>> m_imageImporters;
    std::vector<rcptr<gfx::geom::IGeometryImporter>> m_meshImporters;
    std::vector<rcptr<gfx::geom::IGeometryExporter>> m_meshExporters;
};


static const struct
{
    PluginManager* operator->() const
    {
        static PluginManager instance;
        return &instance;
    }
}
g_pluginMgr;

} // ns mcr
