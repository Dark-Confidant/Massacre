#pragma once

#include <mcr/gfx/IImageImporter.h>
#include <mcr/gfx/IMeshImporter.h>
#include <mcr/gfx/IMeshExporter.h>

namespace mcr {

/*! Plugin interface.

\code{.cpp}
    using namespace mcr;

    class MyPlugin: public IPlugin
    {
    public:
        uint numMeshImporters()
        {
            return 2;
        }

        rcptr<gfx::IMeshImporter> createMeshImporter(uint i) const
        {
            switch (i)
            {
                ...
                return new MyMeshImporter;
            }
            return nullptr;
        }
    };

    MCR_PLUGIN_EXTERN const IPlugin& plugin()
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
    virtual rcptr<gfx::IImageImporter> createImageImporter()
    {
        return nullptr;
    }

    //! Number of implemented mesh importers.
    virtual uint numMeshImporters() const
    {
        return 0;
    }

    //! Create an instance of the ith mesh importer, where 0 <= i < numMeshImporters().
    virtual rcptr<gfx::IMeshImporter> createMeshImporter(uint i) const
    {
        return nullptr;
    }

    //! Number of implemented mesh exporters.
    virtual uint numMeshExporters() const
    {
        return 0;
    }

    //! Create an instance of the ith mesh exporter, where 0 <= i < numMeshExporters().
    virtual rcptr<gfx::IMeshExporter> createMeshExporter(uint i) const
    {
        return nullptr;
    }
};

} // ns mcr
