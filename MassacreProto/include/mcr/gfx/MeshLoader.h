#pragma once

#include <mcr/gfx/IMeshImporter.h>

namespace mcr {

class FileSystem;

namespace gfx {
    
struct Mesh;


struct MeshFileSet
{
    std::vector<std::string> files;

    MeshFileSet& operator()(const char* file)
    {
        files.push_back(file);
        return *this;
    }
};

class MeshLoader
{
public:
    MeshLoader(FileSystem* fs, IMeshImporter* importer = createSimpleMeshImporter());
    ~MeshLoader(); // inherit not

    MCR_EXTERN rcptr<VertexArray> load(const MeshFileSet& files, std::vector<Mesh>& meshesOut);

private:
    FileSystem* m_fs;
    rcptr<IMeshImporter> m_importer;
};


inline MeshLoader::MeshLoader(FileSystem* fs, IMeshImporter* importer):
    m_fs(fs), m_importer(importer) {}

inline MeshLoader::~MeshLoader() {}

} // ns gfx
} // ns mcr
