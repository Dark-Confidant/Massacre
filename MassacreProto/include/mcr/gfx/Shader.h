#pragma once

#include <vector>
#include <mcr/FileSystem.h>
#include <mcr/gfx/IShaderPreprocessor.h>

namespace mcr {
namespace gfx {

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

    //! Create empty shader of given \c type
    static rcptr<Shader> create(Type type);

    /*! Create shader from contents of \c file,
        with type derived from file extension */
    static rcptr<Shader> createFromFile(IFile* file);

    //! Create shader of given type from contents of \c file
    static rcptr<Shader> createFromFile(Type type, IFile* file);


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
    MCR_EXTERN void setSource(const char* source, bool recompile = true);

    //! Set shader source from contents of file and optionally recompile
    void setSourceFromFile(IFile* reader, bool recompile = true);


    //! Compile shader
    MCR_EXTERN bool compile();

    //! Whether shader was compiled successfully
    bool isValid() const { return m_valid; }

    //! Query shader compilation log
    MCR_EXTERN std::string log() const;

protected:
    MCR_EXTERN Shader(Type type);
    MCR_EXTERN ~Shader();

    Type m_type;
    uint m_handle, m_htype;

    IShaderPreprocessor* m_preprocessor;

    std::vector<std::string> m_sources;
    std::vector<const char*> m_sourcePtrs;
    std::vector<int>         m_sourceLengths;

    bool m_valid;
};

} // ns gfx
} // ns mcr

#include "Shader.inl"
