#pragma once

#include <vector>
#include <mcr/GfxExtern.h>
#include <mcr/io/FileSystem.h>
#include <mcr/gfx/mtl/IShaderPreprocessor.h>

namespace mcr {
namespace gfx {
namespace mtl {

class Shader: public RefCounted
{
public:
    enum Type
    {
        Vertex,
        Geometry,
        Fragment,
        NumTypes
    };

    //! Create an empty shader of given \c type
    static rcptr<Shader> create(Type type);


    //! Shader type enum
    Type type() const { return m_type; }

    //! OpenGL object handle
    uint handle() const { return m_handle; }

    //! OpenGL object type enum
    uint handleType() const { return m_htype; }


    //! Preprocessor, naturally
    IShaderPreprocessor* preprocessor() const;

    //! Set preprocessor before setting source
    void setPreprocessor(IShaderPreprocessor* pp);


    //! Shader source strings
    const std::vector<std::string>& sources() const { return m_sources; }

    //! Set shader source and optionally recompile OpenGL binary
    MCR_GFX_EXTERN void setSource(const char* source, bool recompile = true);

    //! Read shader source from the stream and optionally recompile
    void setSourceFromStream(io::IReader* stream, bool recompile = true);


    //! Compile shader
    MCR_GFX_EXTERN bool compile();

    //! Whether shader was compiled successfully
    bool isValid() const { return m_valid; }

    //! Query shader compilation log
    MCR_GFX_EXTERN std::string log() const;

protected:
    MCR_GFX_EXTERN Shader(Type type);
    MCR_GFX_EXTERN ~Shader();

    Type m_type;
    uint m_handle, m_htype;

    IShaderPreprocessor* m_preprocessor;

    std::vector<std::string> m_sources;
    std::vector<const char*> m_sourcePtrs;
    std::vector<int>         m_sourceLengths;

    bool m_valid;
};

} // ns mtl
} // ns gfx
} // ns mcr

#include "Shader.inl"
