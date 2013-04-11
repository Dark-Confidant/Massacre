#pragma once

#include "NonCopyable.h"
#include "Material.h"
#include "math/Rect.h"

namespace mcr {
namespace gfx {

class MaterialManager: NonCopyable
{
public:
    MaterialManager(): m_fs(new FileSystem), m_ownFs(true) {}
    MaterialManager(FileSystem* fs): m_fs(fs), m_ownFs(false) {}

    ~MaterialManager()
    {
        if (m_ownFs)
            delete m_fs;
    }

    Texture*       getTexture(const char* filename);
    Shader*        getShader(const char* filename);
    ShaderProgram* getShaderProgram(const char* vsFilename, const char* fsFilename);
    ShaderProgram* getShaderProgram(const char* vsFilename, const char* gsFilename, const char* fsFilename);
    Material*      getMaterial(const char* filename);

    void clear();
    void removeUnused();

    FileSystem* fs() const { return m_fs; }
    bool hasOwnFs()  const { return m_ownFs; }

    std::map<std::string, rect> parseAtlasTMP(const char* filename);

protected:
    Shader* _getShader(const char* filename, std::string& path);
    ShaderProgram* _getShaderProgram(const std::set<std::string>& filenames);

    void _parseMaterial(Material* mtl, IFile* file);

    template <typename M>
    void _dropAll(M& map)
    {
        for (auto it = map.begin(); it != map.end(); ++it)
            if (it->second->drop())
                it = map.erase(it);
    }

    template <typename M>
    void _grabAll(M& map)
    {
        for (auto it = map.begin(); it != map.end(); ++it)
            it->second->grab();
    }

    FileSystem* m_fs;
    bool m_ownFs;

    std::map<std::string, rcptr<Texture>> m_textures;
    std::map<std::string, rcptr<Shader>> m_shaders;
    std::map<std::string, rcptr<ShaderProgram>> m_programs;
    std::map<std::string, rcptr<Material>> m_materials;
};

} // ns gfx
} // ns mcr
