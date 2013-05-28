#pragma once

#include <mcr/gfx/mtl/IImageImporter.h>
#include <mcr/gfx/geom/IGeometryImporter.h>
#include <mcr/gfx/geom/IGeometryExporter.h>

namespace mcr {

/*! Plugin interface.

\code{.cpp}
    using namespace mcr;

    class MyPlugin: public IPlugin
    {
    public:
        uint numGeometryImporters()
        {
            return 2;
        }

        rcptr<gfx::geom::IGeometryImporter> createGeometryImporter(uint i) const
        {
            switch (i)
            {
                ...
                return new MyGeometryImporter;
            }
            return nullptr;
        }
    };

    MCR_GFX_PLUGIN_EXTERN const IPlugin& plugin()
    {
        static MyPlugin s_plugin;
        return s_plugin;
    }
\endcode
*/
class IPlugin
{
public:
    virtual ~IPlugin() {}

    //! Number of implemented image importers.
    virtual uint numImageImporters() const
    {
        return 0;
    }

    //! Create an instance of the ith image importer, where 0 <= i < numImageImporters().
    virtual rcptr<gfx::mtl::IImageImporter> createImageImporter(uint i) const
    {
        return nullptr;
    }

    //! Number of implemented geometry importers.
    virtual uint numGeometryImporters() const
    {
        return 0;
    }

    //! Create an instance of the ith geometry importer, where 0 <= i < numGeometryImporters().
    virtual rcptr<gfx::geom::IGeometryImporter> createGeometryImporter(uint i) const
    {
        return nullptr;
    }

    //! Number of implemented geometry exporters.
    virtual uint numGeometryExporters() const
    {
        return 0;
    }

    //! Create an instance of the ith geometry exporter, where 0 <= i < numGeometryExporters().
    virtual rcptr<gfx::geom::IGeometryExporter> createGeometryExporter(uint i) const
    {
        return nullptr;
    }
};

} // ns mcr
