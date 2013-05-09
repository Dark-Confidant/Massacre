// Internal header
#pragma once

#include <vector>
#include <mcr/Types.h>

namespace mcr {
namespace gfx {

class GLState
{
public:
    GLState();

    uint    activeTexUnit() const;
    void    setActiveTexUnit(uint unit);
    uint    boundTexture() const;
    uint    boundTexture(uint unit) const;
    void    bindTexture(uint tex);
    void    bindTexture(uint unit, uint tex);

    uint    activeProgram() const;
    void    setActiveProgram(uint program);

    uint    boundBuffer(uint target) const;
    uint    boundBuffer(uint target, uint index) const;
    void    bindBuffer(uint target, uint buffer);
    void    bindBufferBase(uint target, uint index, uint buffer);

    uint    boundVertexArray() const;
    void    bindVertexArray(uint va);

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

    std::vector<uint>   m_texUnits;
    uint                m_activeTexUnit;

    uint                m_activeProgram;

    std::vector<uint>   m_buffersIndexed[NumIndexedBufferTargets];
    uint                m_buffers       [NumBufferTargets];

    std::vector<VAO>    m_vertexArrays;
    uint                m_activeVertexArray;
};


static const struct
{
    GLState* operator->() const
    {
        static GLState instance;
        return &instance;
    }
}
g_glState;

} // ns gfx
} // ns mcr
