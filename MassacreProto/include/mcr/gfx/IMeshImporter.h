#pragma once

#include <mcr/FileSystem.h>
#include <mcr/gfx/Mesh.h>
#include <mcr/gfx/VertexFormat.h>

namespace mcr {
namespace gfx {

class IMeshImportTask: public RefCounted
{
public:
    struct MeshInfo
    {
        VertexFormat vertexFormat;
        uint numVertices;
        uint numIndices;
    };

    /*! Determine mesh parameters. Good point to check signature and read headers.
        \param infoOut Must be filled out.
        \return Import possibility (e.g. whether signatures are valid).
    */
    virtual bool estimate(MeshInfo& infoOut) = 0;

    /*! Load the mesh, if possible.
        \param bufferFormat Format of the buffer that contains the mesh.
        \param mesh The mesh to be populated.
        \return Import status (e.g. whether the mesh is properly populated).
    */
    virtual bool import(const VertexFormat& bufferFormat, Mesh& mesh) = 0;
};

class IMeshImporter: public RefCounted
{
public:
    //! Create an import task
    virtual rcptr<IMeshImportTask> createTask(IFile* file) const = 0;
};

MCR_EXTERN rcptr<IMeshImporter> createSimpleMeshLoader();

} // ns gfx
} // ns mcr
