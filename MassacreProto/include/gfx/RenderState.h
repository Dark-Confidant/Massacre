#pragma once

#include <MassacreTypes.h>

namespace mcr {
namespace gfx {

struct DepthFn
{
    enum Fn: uint
    {
        Never,
        Less,
        Equal,
        LEqual,
        Greater,
        NotEqual,
        GEqual,
        Always,
        NumFns
    }
    fn;

    DepthFn(): fn(Less) {}
    DepthFn(Fn func): fn(func) {}
    DepthFn(uint func): fn(Fn(func)) {}
};

struct BlendFn
{
    enum Factor: uint
    {
        Zero, One,
        SrcColor, OneMinusSrcColor,
        SrcAlpha, OneMinusSrcAlpha,
        DstAlpha, OneMinusDstAlpha,
        DstColor, OneMinusDstColor,
        SrcAlphaSaturate,
        NumFactors
    }
    srcFactor, dstFactor;

    BlendFn(): srcFactor(One), dstFactor(Zero) {}
    BlendFn(Factor src, Factor dst): srcFactor(src), dstFactor(dst) {}
    BlendFn(uint src, uint dst): srcFactor(Factor(src)), dstFactor(Factor(dst)) {}
};

struct RenderState
{
    bool depthTest, depthWrite;
    DepthFn depthFunc;

    bool alphaTest;

    bool blend;
    BlendFn blendFunc;

    bool cullFace;
    bool polygonOffset;

    RenderState();
    explicit RenderState(uint hash);
    ~RenderState() {}

    uint hash() const;
};

} // ns gfx
} // ns mcr

#include "RenderState.inl"
