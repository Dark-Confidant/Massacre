#pragma once

#include <string>
#include <vector>
#include <map>
#include <mcr/RefCounted.h>
#include <mcr/gfx/MaterialParameter.h>

namespace mcr {
namespace gfx {

struct MaterialParameterBufferLayout
{
    std::vector<std::pair<MaterialParameterType, std::string>> parameters;

    MaterialParameterBufferLayout& operator()(MaterialParameterType type, const char* pname)
    {
        parameters.push_back(std::make_pair(type, pname));
        return *this;
    }
};

class MaterialParameterBuffer: public RefCounted
{
public:
    enum Usage: uint {Static, Dynamic, Stream};

    static rcptr<MaterialParameterBuffer> create(
        const char* name,
        const MaterialParameterBufferLayout& layout,
        Usage usage = Dynamic);


    int                                     findParameter(const std::string& pname) const;

    const IMaterialParameter&               parameter(const std::string& pname) const;
    IMaterialParameter&                     parameter(const std::string& pname);

    const IMaterialParameter&               parameter(int index) const;
    IMaterialParameter&                     parameter(int index);

    const IMaterialParameter&               operator[](const std::string& pname) const;
    IMaterialParameter&                     operator[](const std::string& pname);

    const IMaterialParameter&               operator[](int index) const;
    IMaterialParameter&                     operator[](int index);

    const std::string&                      name() const;
    const MaterialParameterBufferLayout&    layout() const;

    Usage                                   usage() const;
    void                                    setUsage(Usage usage);

    uint                                    handle() const;

    MCR_EXTERN void                         init();
    MCR_EXTERN void                         sync();

    const std::string&                      glslDefinition() const;

protected:
    MaterialParameterBuffer(const char* name, const MaterialParameterBufferLayout& layout, Usage usage);
    ~MaterialParameterBuffer();

private:
    class Parameter: public IMaterialParameter
    {
    public:
        Parameter(MaterialParameterType type, void* mem, bool* dirty);

    private:
        const void* mem() const;
        void*       mem();
        void        invalidate();

        void* m_mem;
        bool* m_dirty;
    };

    const std::string m_name;
    const MaterialParameterBufferLayout m_layout;
    std::string m_glslDef;

    std::vector<Parameter> m_parameters;
    std::map<std::string, std::size_t> m_parametersByName;
    std::size_t m_bufferSize;
    byte* m_buffer;
    Usage m_usage;
    bool m_dirty;

    // implementation details
    uint m_handle;
};

} // ns gfx
} // ns mcr

#include "MaterialParameterBuffer.inl"
