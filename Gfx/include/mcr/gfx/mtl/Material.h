#pragma once

#include <vector>
#include <mcr/gfx/mtl/RenderState.h>
#include <mcr/gfx/mtl/Shader.h>
#include <mcr/gfx/mtl/Texture.h>
#include <mcr/gfx/mtl/ParamBuffer.h>

namespace mcr {
namespace gfx {
namespace mtl {

struct ShaderList
{
    std::vector<rcptr<Shader>> shaders;

    ShaderList& add(Shader* shader)
    {
        shaders.push_back(shader);
        return *this;
    }
};

class Manager;

class Material: public RefCounted
{
public:
    static rcptr<Material>  create(Manager* mgr);

    const RenderState&      renderState() const;
    uint                    renderStateHash() const;
    void                    setRenderState(const RenderState& state);
                                
    template                <typename T>
    void                    set(T RenderState::* param, const T& val);
                                
    const ShaderList&       shaders() const;
    bool                    setShaders(const ShaderList& shaders);

    const mtl::IParam&      param(const std::string& pname) const;
    mtl::IParam&            param(const std::string& pname);

    byte                    numTextures() const;
    Texture*                texture(byte idx) const;
    bool                    setTexture(byte idx, Texture* tex);

    int                     passHint() const;
    void                    setPassHint(int pass);

    MCR_GFX_EXTERN void     syncParams();

private:
    class Param: public mtl::IParam
    {
    public:
        typedef void(*UploadFn)(uint program, int loc, const void* mem);

        Param(mtl::ParamType type, uint program, int loc, UploadFn uploadFn);
        ~Param();

        void sync();

    private:
        const void*         mem() const;
        void*               mem();
        MCR_GFX_EXTERN void invalidate();

        uint     m_program;
        int      m_location;
        byte*    m_data;
        UploadFn m_uploadFn;
        bool     m_dirty;
    };

    MCR_GFX_EXTERN Material(Manager* mgr);
    MCR_GFX_EXTERN ~Material();

    MCR_GFX_EXTERN bool _link();

    Manager*            m_mgr;
    RenderState         m_renderState;
    uint                m_renderStateHash;
    ShaderList          m_shaders;
    std::vector<Param>  m_params;
    int                 m_passHint;

    std::vector<std::pair<uint, rcptr<mtl::ParamBuffer>>> m_buffers;
    std::vector<std::pair<uint, rcptr<Texture>>>          m_textures;

    // implementation details
    uint m_program;
};

} // ns mtl
} // ns gfx
} // ns mcr

#include "Material.inl"
