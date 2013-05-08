#pragma once

#include <mcr/NonCopyable.h>
#include <mcr/math/Rect.h>
#include <mcr/gfx/Material.h>
#include <mcr/gfx/MaterialParameterBuffer.h>

namespace mcr {
namespace gfx {

class MaterialManager: NonCopyable
{
public:
    MaterialManager();
    MaterialManager(FileSystem* fs);

    ~MaterialManager(); // inherit not


    // Parameter buffer management

    uint requestParameterBufferBinding();

    bool addParameterBuffer(MaterialParameterBuffer* buffer);
    bool addParameterBuffer(const std::string& name, MaterialParameterBuffer* buffer);

    void removeParameterBuffer(MaterialParameterBuffer* buffer);
    void removeParameterBuffer(const std::string& name);

    MaterialParameterBuffer* parameterBuffer(const std::string& name) const;


    // Texture units management

    uint requestTexUnit();


    // Resource management

    Texture*    getTexture(const char* filename);
    Shader*     getShader(const char* filename);
    Material*   getMaterial(const char* filename);

    void        clear();
    void        removeUnused();

    FileSystem* fs() const;
    bool        hasOwnFs()  const;

    std::map<std::string, rect> parseAtlasTMP(const char* filename);

private:
    MCR_EXTERN void     _init();
    MCR_EXTERN void     _destroy();

    MCR_INTERN Shader*  _getShader(const char* filename, std::string& path);
    MCR_INTERN void     _parseMaterial(Material* mtl, IFile* file);

    template <typename M> void _dropAll(M& map);
    template <typename M> void _grabAll(M& map);

    FileSystem* m_fs;
    bool m_ownFs;

    IShaderPreprocessor* m_preprocessor;

    struct TextureData
    {
        struct Unit
        {
            Texture* tex;
        };

        std::map<std::string, rcptr<Texture>> textures;
        std::vector<Unit> units;
        std::size_t nextFreeUnit;

        MCR_EXTERN TextureData();
    }
    m_tex;

    struct ParamBufferData
    {
        std::map<std::string, rcptr<MaterialParameterBuffer>> buffers;
        uint numBindings, nextFreeBinding;

        MCR_EXTERN ParamBufferData();
    }
    m_pb;

    std::map<std::string, rcptr<Shader>> m_shaders;
    std::map<std::string, rcptr<Material>> m_materials;
};

} // ns gfx
} // ns mcr

#include "MaterialManager.inl"
