#pragma once

#include <vector>
#include "math/Matrix.h"
#include "GLUtility.h"
#include "FileSystem.h"

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

    struct ConstantInfo
    {
        GlslType type;
        uint stringIdx;
        double minValue, maxValue;
        std::string defaultValue;
        std::string name, description;
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


    // TODO: document

    int findConstant(const char* name) const;

    const ConstantInfo& constantInfo(int idx) const;

    const std::string& constantValue(int idx) const;
    const std::string& constantValue(const char* name) const;

    MCR_EXTERN void replaceConstant(int idx, const std::string& value);

    template <typename T>
    void replaceConstant(int idx, const T& val);

    template <typename T>
    void replaceConstant(const char* name, T val);

protected:
    MCR_EXTERN Shader(Type type);
    MCR_EXTERN ~Shader();

    MCR_INTERN void _feedSource();
    MCR_INTERN void _preprocessSource(const char* source);
    MCR_INTERN void _addSourceString(const char* first, const char* last);
    MCR_INTERN bool _parseCommand(const char* str, const char*& cmdfirst, const char*& cmdlast, ConstantInfo& info);
    MCR_INTERN void _parseType(const char* str, GlslType& type);

    Type m_type;
    uint m_handle, m_htype;

    std::vector<std::string> m_sources;
    std::vector<const char*> m_sourcePtrs;
    std::vector<int>         m_sourceLengths;

    std::vector<ConstantInfo> m_constants;

    bool m_valid;
};

} // ns gfx
} // ns mcr

#include "Shader.inl"
