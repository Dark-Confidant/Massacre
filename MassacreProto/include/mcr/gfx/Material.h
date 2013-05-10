#pragma once

#include <vector>
#include <mcr/gfx/RenderState.h>
#include <mcr/gfx/Shader.h>
#include <mcr/gfx/MaterialParameterBuffer.h>
#include <mcr/gfx/Texture.h>

namespace mcr {
namespace gfx {

struct MaterialShaderSet
{
    std::vector<rcptr<Shader>> shaders;

    MaterialShaderSet& operator()(Shader* shader)
    {
        shaders.push_back(shader);
        return *this;
    }
};

class MaterialManager;

class Material: public RefCounted
{
public:
    static rcptr<Material>      create(MaterialManager* mgr);

    const RenderState&          renderState() const;
    uint                        renderStateHash() const;
    void                        setRenderState(const RenderState& state);
                                
    template                    <typename T>
    void                        set(T RenderState::* param, const T& val);
                                
    const MaterialShaderSet&    shaders() const;
    bool                        setShaders(const MaterialShaderSet& shaders);

    const IMaterialParameter&   parameter(const std::string& pname) const;
    IMaterialParameter&         parameter(const std::string& pname);

    byte                        numTextures() const;
    Texture*                    texture(byte idx) const;
    bool                        setTexture(byte idx, Texture* tex);

    int                         passHint() const;
    void                        setPassHint(int pass);

    MCR_EXTERN void             syncParameters();

private:
    class Parameter: public IMaterialParameter
    {
    public:
        typedef void(*UploadFn)(uint program, int loc, const void* mem);

        Parameter(MaterialParameterType type, uint program, int loc, UploadFn uploadFn);
        ~Parameter();

        void sync();

    private:
        const void*     mem() const;
        void*           mem();
        MCR_EXTERN void invalidate();

        uint     m_program;
        int      m_location;
        byte*    m_data;
        UploadFn m_uploadFn;
        bool     m_dirty;
    };

    MCR_EXTERN Material(MaterialManager* mgr);
    MCR_EXTERN ~Material();

    MCR_EXTERN bool _link();

    MaterialManager*            m_mgr;
    RenderState                 m_renderState;
    uint                        m_renderStateHash;
    MaterialShaderSet           m_shaders;
    std::vector<Parameter>      m_params;
    int                         m_passHint;

    std::vector<std::pair<uint, rcptr<MaterialParameterBuffer>>> m_buffers;
    std::vector<std::pair<uint, rcptr<Texture>>>                 m_textures;

    // implementation details
    uint m_program;
};

} // ns gfx
} // ns mcr

#include "Material.inl"
