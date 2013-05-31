#pragma once

#include <mcr/GfxExtern.h>
#include <mcr/NonCopyable.h>
#include <mcr/math/Rect.h>
#include <mcr/io/FileSystem.h>
#include <mcr/gfx/mtl/Material.h>

namespace mcr {
namespace gfx {
namespace mtl {

class Manager: NonCopyable
{
public:
    Manager();
    Manager(io::FileSystem* fs);

    ~Manager(); // inherit not


    // Parameter buffer management

    uint requestParamBufferBinding();

    bool addParamBuffer(mtl::ParamBuffer* buffer);
    bool addParamBuffer(const std::string& name, mtl::ParamBuffer* buffer);

    void removeParamBuffer(mtl::ParamBuffer* buffer);
    void removeParamBuffer(const std::string& name);

    mtl::ParamBuffer* paramBuffer(const std::string& name) const;


    // Texture units management

    uint requestTexUnit();


    // Resource management

    MCR_GFX_EXTERN Texture*     getTexture(const std::string& filename);
    MCR_GFX_EXTERN Shader*      getShader(const std::string& filename);
    MCR_GFX_EXTERN Material*    getMaterial(const std::string& filename);

    MCR_GFX_EXTERN void         clear();
    MCR_GFX_EXTERN void         removeUnused();

    io::FileSystem*             fs() const;
    bool                        hasOwnFs()  const;

private:
    MCR_GFX_EXTERN void         _init();
    MCR_GFX_EXTERN void         _destroy();

    MCR_GFX_INTERN void         _parseMaterial(Material* material, io::IReader* stream);

    template <typename M> void  _dropAll(M& map);
    template <typename M> void  _grabAll(M& map);

    io::FileSystem* m_fs;
    bool            m_ownFs;

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

        MCR_GFX_EXTERN TextureData();
    }
    m_tex;

    struct ParamBufferData
    {
        std::map<std::string, rcptr<mtl::ParamBuffer>> buffers;
        uint numBindings, nextFreeBinding;

        MCR_GFX_EXTERN ParamBufferData();
    }
    m_pb;

    std::map<std::string, rcptr<Shader>>   m_shaders;
    std::map<std::string, rcptr<Material>> m_materials;
};

} // ns mtl
} // ns gfx
} // ns mcr

#include "Manager.inl"
