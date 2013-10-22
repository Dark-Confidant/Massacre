namespace mcr {
namespace gfx {
namespace mtl {

inline ParamLayout& ParamLayout::add(ParamType type, const char* pname)
{
    params.push_back(std::make_pair(type, pname));
    return *this;
}

inline ParamLayout& ParamLayout::addFloat(const char* pname)
{
    return add(ParamType::Float, pname);
}

inline ParamLayout& ParamLayout::addVec2(const char* pname)
{
    return add(ParamType::Vec2, pname);
}

inline ParamLayout& ParamLayout::addVec3(const char* pname)
{
    return add(ParamType::Vec3, pname);
}

inline ParamLayout& ParamLayout::addVec4(const char* pname)
{
    return add(ParamType::Vec4, pname);
}

inline ParamLayout& ParamLayout::addDouble(const char* pname)
{
    return add(ParamType::Double, pname);
}

inline ParamLayout& ParamLayout::addDVec2(const char* pname)
{
    return add(ParamType::DVec2, pname);
}

inline ParamLayout& ParamLayout::addDVec3(const char* pname)
{
    return add(ParamType::DVec3, pname);
}

inline ParamLayout& ParamLayout::addDVec4(const char* pname)
{
    return add(ParamType::DVec4, pname);
}

inline ParamLayout& ParamLayout::addInt(const char* pname)
{
    return add(ParamType::Int, pname);
}

inline ParamLayout& ParamLayout::addIVec2(const char* pname)
{
    return add(ParamType::IVec2, pname);
}

inline ParamLayout& ParamLayout::addIVec3(const char* pname)
{
    return add(ParamType::IVec3, pname);
}

inline ParamLayout& ParamLayout::addIVec4(const char* pname)
{
    return add(ParamType::IVec4, pname);
}

inline ParamLayout& ParamLayout::addUInt(const char* pname)
{
    return add(ParamType::UInt, pname);
}

inline ParamLayout& ParamLayout::addUVec2(const char* pname)
{
    return add(ParamType::UVec2, pname);
}

inline ParamLayout& ParamLayout::addUVec3(const char* pname)
{
    return add(ParamType::UVec3, pname);
}

inline ParamLayout& ParamLayout::addUVec4(const char* pname)
{
    return add(ParamType::UVec4, pname);
}

inline ParamLayout& ParamLayout::addMat4(const char* pname)
{
    return add(ParamType::Mat4, pname);
}

inline ParamLayout& ParamLayout::addDMat4(const char* pname)
{
    return add(ParamType::DMat4, pname);
}

inline std::size_t ParamLayout::totalSize() const
{
    std::size_t size = 0;

    for (std::size_t i = 0; i < params.size(); ++i)
        size += params[i].first.size();

    return size;
}

inline std::size_t ParamLayout::totalSizeAligned() const
{
    std::size_t size = 0;

    for (std::size_t i = 0; i < params.size(); ++i)
        size += params[i].first.sizeAligned();

    return size;
}

} // ns mtl
} // ns gfx
} // ns mcr
