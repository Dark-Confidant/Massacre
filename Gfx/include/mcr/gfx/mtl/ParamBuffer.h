#pragma once

#include <string>
#include <vector>
#include <map>
#include <mcr/GfxExtern.h>
#include <mcr/RefCounted.h>
#include <mcr/gfx/mtl/ParamLayout.h>

namespace mcr {
namespace gfx {
namespace mtl {

class ParamBuffer: public RefCounted
{
public:
    enum Usage: uint {Static, Dynamic, Stream};

    static rcptr<ParamBuffer> create(
        const char* name,
        const ParamLayout& layout,
        Usage usage = Stream);


    int                 findParam(const std::string& pname) const;

    const IParam&       param(const std::string& pname) const;
    IParam&             param(const std::string& pname);

    const IParam&       param(int index) const;
    IParam&             param(int index);

    const IParam&       operator[](const std::string& pname) const;
    IParam&             operator[](const std::string& pname);

    const IParam&       operator[](int index) const;
    IParam&             operator[](int index);

    const std::string&  name() const;
    const ParamLayout&  layout() const;

    Usage               usage() const;
    void                setUsage(Usage usage);

    uint                handle() const;

    MCR_GFX_EXTERN void init();
    MCR_GFX_EXTERN void sync();

protected:
    MCR_GFX_EXTERN ParamBuffer(const char* name, const ParamLayout& layout, Usage usage);
    MCR_GFX_EXTERN ~ParamBuffer();

private:
    class Param: public IParam
    {
    public:
        Param(ParamType type, void* mem, bool* dirty);

    private:
        const void* mem() const;
        void*       mem();
        void        invalidate();

        void* m_mem;
        bool* m_dirty;
    };

    const std::string m_name;
    const ParamLayout m_layout;

    std::vector<Param>                  m_params;
    std::map<std::string, std::size_t>  m_paramsByName;

    std::size_t m_bufferSize;
    byte*       m_buffer;
    Usage       m_usage;
    bool        m_dirty;

    // implementation details
    uint        m_handle;
};

} // ns mtl
} // ns gfx
} // ns mcr

#include "ParamBuffer.inl"
