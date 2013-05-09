#pragma once

#include <mcr/RefCounted.h>

namespace mcr {
namespace gfx {

class GBuffer: public RefCounted
{
public:
    enum Type
    {
        ArrayBuffer,
        ElementArrayBuffer,
        UniformBuffer,
        TextureBuffer,
        TransformFeedbackBuffer,
        //AtomicCounterBuffer,
        //CopyReadBuffer,
        //CopyWriteBuffer,
        //DrawIndirectBuffer,
        //DispatchIndirectBuffer,
        //PixelPackBuffer,
        //PixelUnpackBuffer,
        //ShaderStorageBuffer,
        NumTypes
    };

    enum Usage
    {
        StreamDraw,  StreamRead,  StreamCopy,
        StaticDraw,  StaticRead,  StaticCopy,
        DynamicDraw, DynamicRead, DynamicCopy
    };

    enum Access: ushort
    {
        Read             = 1 << 0,
        Write            = 1 << 1,
        InvalidateRange  = 1 << 2,
        InvalidateBuffer = 1 << 3,
        FlushExplicit    = 1 << 4,
        Unsynchronized   = 1 << 5,

        ReplaceSome = Write | InvalidateRange,
        ReplaceAll  = Write | InvalidateBuffer
    };

    static rcptr<GBuffer> create(Type type);
    static rcptr<GBuffer> create(Type type, uint size, Usage usage);

    void init(uint size, Usage usage);
    void init(const void* data, uint size, Usage usage);

    Type type() const;
    uint handle() const;

    uint capacity() const;

    void* map(uint access);
    void* map(uint offset, uint length, uint access);

    const void* map(uint access) const;
    const void* map(uint offset, uint length, uint access) const;

    void unmap() const;

protected:
    GBuffer(Type type);
    ~GBuffer();

    Type m_type;
    uint m_capacity;
    uint m_handle;
};

} // ns gfx
} // ns mcr

#include "GBuffer.inl"
