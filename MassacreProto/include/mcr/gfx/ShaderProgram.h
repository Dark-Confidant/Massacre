#pragma once

#include <vector>
#include <map>
#include <mcr/gfx/GLUtility.h>
#include <mcr/gfx/Shader.h>

namespace mcr {
namespace gfx {

class ShaderProgram: public RefCounted
{
public:
    struct UniformBlockInfo
    {
        std::string name;
        int size;
    };

    struct SamplerInfo
    {
        std::string name;
        uint unit, target;
    };

    //! Create empty program
    static rcptr<ShaderProgram> create();

    //! Create and link program with vertex and frament shaders attached
    static rcptr<ShaderProgram> create(Shader* vert, Shader* frag);

    //! Create and link program with vertex, geometry, and fragment shaders attached
    static rcptr<ShaderProgram> create(Shader* vert, Shader* geom, Shader* frag);

    /*! Create and link program with vertex and fragment shaders loaded from files
        Note that filename extensions are ignored,
        i.e. \c vert named 'something.frag' will be treated as vertex shader
    */
    static rcptr<ShaderProgram> createFromFiles(IFile* vert, IFile* frag);

    /*! Create and link program with verex, geometry, and fragment shaders loaded from files
        \sa createFromFiles(IFile* vert, IFile* frag)
    */
    static rcptr<ShaderProgram> createFromFiles(IFile* vert, IFile* geom, IFile* frag);


    //! OpenGL object handle
    uint handle() const { return m_handle; }


    /*! Add shader to program
        There must be at least vertex and fragment shaders attached to program to link it.
        \return Attachment index
    */
    byte attach(Shader* shader);

    //! Remove previously added shader from program
    void detach(Shader* shader);

    //! Remove previously added shader from program by attachment index
    void detach(byte idx);


    //! Number of shaders attached
    byte numShaders() const { return (byte) m_shaders.size(); }

    //! Shader by attachment index
    Shader* shader(byte idx) { return m_shaders[idx]; }


    //! Try to link program
    bool link();

    //! Whether link succeeded
    bool isValid() const { return m_valid; }

    //! Query link log
    std::string log() const;


    // sampler management - FIXME

    int             numSamplers()        const { return (int) m_samplers.size(); }
    const SamplerInfo& sampler (int idx) const { return       m_samplers[idx];   }


    //! Find uniform by name
    int findUniform(const char* name);

    //! Appies to active program set by Context::setActiveProgram
    struct Active
    {
        //! Set uniform
        template <typename T>
        static void setUniform(int location, const T& value);
    };

protected:
    ShaderProgram();
    ~ShaderProgram();

    std::vector<rcptr<Shader>> m_shaders;
    uint m_handle;
    bool m_valid;

    std::vector<SamplerInfo> m_samplers;
    std::map<std::string, int> m_locations;

    std::vector<UniformBlockInfo> m_blocks;
};

} // ns gfx
} // ns mcr
