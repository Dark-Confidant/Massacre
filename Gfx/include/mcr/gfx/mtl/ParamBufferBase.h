#pragma once

#include <string>
#include <vector>
#include <map>
#include <mcr/RefCounted.h>
#include <mcr/NonCopyable.h>
#include <mcr/gfx/mtl/ParamLayout.h>

namespace mcr {
namespace gfx {
namespace mtl {

class ParamBufferBase: public RefCounted, NonCopyable
{
public:
    int                         numParams() const;
    const std::string&          paramName(int index) const;
    ParamType                   paramType(int index) const;
    int                         findParam(const std::string& pname) const;

    // These call findParam(pname) under the hood, each time
    template <typename T> bool  getParam(const std::string& pname, T& valueOut) const;
    template <typename T> bool  setParam(const std::string& pname, const T& value);

    template <typename T> bool  getParam(int index, T& valueOut) const;
    template <typename T> bool  setParam(int index, const T& value);

protected:
    ParamBufferBase() {}
    ~ParamBufferBase() {}

    const ParamLayout& layout() const;
    void setLayout(const ParamLayout& layout, bool aligned = true);

    const std::vector<byte>& data() const;

private:
    virtual void onInvalidateParam(int index, const void* data) = 0;

    ParamLayout         m_layout;
    std::vector<void*>  m_params;
    std::vector<byte>   m_data;

    std::map<std::string, std::size_t> m_paramsByName;
};

} // ns mtl
} // ns gfx
} // ns mcr

#include "ParamBufferBase.inl"
