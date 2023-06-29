// Internal header
#pragma once

#include <vector>
#include <string>
#include <mcr/math/Rect.h>

namespace mcr {
namespace gfx {

class GLState
{
public:
    GLState();

    const irect&       viewport() const;
    void               setViewport(const irect& vp);

    uint               activeTexUnit() const;
    void               setActiveTexUnit(uint unit);
    uint               boundTexture() const;
    uint               boundTexture(uint unit) const;
    void               bindTexture(uint tex);
    void               bindTexture(uint unit, uint tex);

    uint               activeProgram() const;
    void               setActiveProgram(uint program);

    uint               boundBuffer(uint target) const;
    uint               boundBuffer(uint target, uint index) const;
    void               bindBuffer(uint target, uint buffer);
    void               bindBufferBase(uint target, uint index, uint buffer);

    uint               boundVertexArray() const;
    void               bindVertexArray(uint va);

    const std::string& renderer() const;
    const std::string& vendor() const;

private:
    enum IndexedBufferTarget
    {
        UniformBuffer,
        TransformFeedbackBuffer,
        //AtomicCounterBuffer,
        //ShaderStorageBuffer,
        NumIndexedBufferTargets
    };
    enum BufferTarget
    {
        ArrayBuffer = NumIndexedBufferTargets,
        ElementArrayBuffer,
        TextureBuffer,
        //CopyReadBuffer,
        //CopyWriteBuffer,
        //DrawIndirectBuffer,
        //DispatchIndirectBuffer,
        //PixelPackBuffer,
        //PixelUnpackBuffer,
        NumBufferTargets
    };

    struct VAO
    {
        union
        {
            struct { uint vertexBuffer, indexBuffer; };
            uint buffers[2];
        };

        VAO(): vertexBuffer(0), indexBuffer(0) {}
    };

    static uint bufferTargetEnumToIndex(uint target);

    irect               m_viewport;

    std::vector<uint>   m_texUnits;
    uint                m_activeTexUnit;

    uint                m_activeProgram;

    std::vector<uint>   m_buffersIndexed[NumIndexedBufferTargets];
    uint                m_buffers       [NumBufferTargets];

    std::vector<VAO>    m_vertexArrays;
    uint                m_activeVertexArray;

    std::string         m_vendorString;
    std::string         m_rendererString;
};


namespace detail {
struct GLStateSingleton
{
    GLStateSingleton() {}

    GLState& operator*() const
    {
        static GLState instance;
        return instance;
    }

    GLState* operator->() const
    {
        return &**this;
    }
};
} // ns detail;

static const detail::GLStateSingleton g_glState;

} // ns gfx
} // ns mcr
