namespace mcr {
namespace gfx {

inline bool operator==(const BlendFn& lhs, const BlendFn& rhs)
{
    return lhs.srcFactor == rhs.srcFactor && lhs.dstFactor == rhs.dstFactor;
}

inline bool operator!=(const BlendFn& lhs, const BlendFn& rhs)
{
    return !(lhs == rhs);
}


inline RenderState::RenderState():
    depthTest    (true),
    depthWrite   (true),
    depthFunc    (DepthFn::LEqual),
    alphaTest    (false),
    blend        (false),
    blendFunc    (BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha),
    cullFace     (true),
    polygonOffset(false) {}

inline RenderState::RenderState(uint hash):
    depthTest    ((hash & 0x1) != 0),
    depthWrite   ((hash & 0x2) != 0),
    depthFunc    ((hash >> 6) & 0x7),
    alphaTest    ((hash & 0x4) != 0),
    blend        ((hash & 0x8) != 0),
    blendFunc    ((hash >> 9) & 0xF, (hash >> 13) & 0xF),
    cullFace     ((hash & 0x10) != 0),
    polygonOffset((hash & 0x20) != 0) {}

inline uint RenderState::hash() const
{
    return uint(depthTest)
        | (uint(depthWrite)                << 1)
        | (uint(alphaTest)                 << 2)
        | (uint(blend)                     << 3)
        | (uint(cullFace)                  << 4)
        | (uint(polygonOffset)             << 5)
        | (uint(depthFunc.fn & 0x7)        << 6)
        | (uint(blendFunc.srcFactor & 0xF) << 9)
        | (uint(blendFunc.dstFactor & 0xF) << 13);
}

} // ns gfx
} // ns mcr
