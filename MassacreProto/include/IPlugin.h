#pragma once

#include <gfx/experimental/IMeshImporter.h>
#include <gfx/experimental/IMeshExporter.h>

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

        rcptr<gfx::experimental::IMeshImporter> createMeshImporter(uint i) const
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

    //! Number of implemented mesh importers.
    virtual uint numMeshImporters() const
    {
        return 0;
    }

    //! Create ith mesh importer, where 0 <= i < numMeshImporters().
    virtual rcptr<gfx::experimental::IMeshImporter> createMeshImporter(uint i) const
    {
        return nullptr;
    }

    //! Number of implemented mesh exporters.
    virtual uint numMeshExporters() const
    {
        return 0;
    }

    //! Create ith mesh exporter, where 0 <= i < numMeshExporters().
    virtual rcptr<gfx::experimental::IMeshExporter> createMeshExporter(uint i) const
    {
        return nullptr;
    }
};

} // ns mcr
