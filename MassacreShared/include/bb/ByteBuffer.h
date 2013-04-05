#pragma once

#include <vector>
#include <stack>
#include <list>
#include "math/Matrix.h" // matrices, vectors, limits, no min/max macros
#include "RefCounted.h"
#include "Manips.h"

namespace mcr {
namespace bb  {

class ByteBuffer: public RefCounted
{
public:
    static rcptr<ByteBuffer> create(uint size = 0) { return new ByteBuffer(size); }


    std::vector<byte>&       contents()       { return m_buffer; }
    const std::vector<byte>& contents() const { return m_buffer; }

    uint size() const { return (uint) m_buffer.size(); }
    void resize(uint size) { m_buffer.resize(size); }


    uint readPos() const { return m_readPos; }
    void setReadPos(uint pos) { m_readPos = pos; }

    uint writePos() const { return m_writePos; }
    void setWritePos(uint pos) { m_writePos = pos; }


    template <typename T>
    T& at(uint offset) { return *(T*) &m_buffer[offset]; }

    template <typename T>
    const T& at(uint offset) const { return *(const T*) &m_buffer[offset]; }


    template <typename T>
    const T& peek() const { return at<T>(m_readPos); }

    template <typename T>
    const T& get()
    {
        auto& result = peek<T>();
        m_readPos += sizeof(T);
        return result;
    }


    template <typename T>
    ByteBuffer& operator>>(T& val)
    {
        val = get<T>();
        return *this;
    }

    template <typename C>
    ByteBuffer& operator>>(std::basic_string<C>& val)
    {
        val.resize(get<uint>());
        val.assign((const C*) &m_buffer[m_readPos], val.size());
        return *this;
    }

    template <typename T>
    ByteBuffer& operator>>(std::vector<T>& vals)
    {
        for (auto count = get<uint>(); count--;)
        {
            vals.push_back(T());
            *this >> vals.back();
        }
        return *this;
    }

    template <typename T>
    ByteBuffer& operator>>(std::list<T>& vals)
    {
        for (auto count = get<uint>(); count--;)
        {
            vals.push_back(T());
            *this >> vals.back();
        }
        return *this;
    }


    ByteBuffer& operator<<(sbyte   val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(short   val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(int     val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(int64   val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(byte    val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(ushort  val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(uint    val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(uint64  val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(float   val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(double  val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(char    val) { return putPod(val).inc(); }
    ByteBuffer& operator<<(wchar_t val) { return putPod(val).inc(); }

    ByteBuffer& operator<<(const char*    str) { return putString(str, strlen(str)).inc(); }
    ByteBuffer& operator<<(const wchar_t* str) { return putString(str, wcslen(str)).inc(); }

    template <typename T>
    ByteBuffer& operator<<(const std::basic_string<T>& str)
    { return putString(str.c_str(), str.size()).inc(); }

    ByteBuffer& operator<<(const DataBlock& data)
    { return putBlock(data.ptr, data.size).inc(); }

    template <typename T>
    ByteBuffer& operator<<(const DataList<T>& data)
    {
        auto numPos = m_writePos;
        putPod(uint());

        uint num = 0;

        for (auto it = data.begin; it != data.end; ++it, ++num)
            putPod(*it);

        at<uint>(numPos) = num;
        return inc();
    }

    template <typename T>
    ByteBuffer& operator<<(const counter<T>& ctr)
    {
        m_counters.push(CounterImpl(m_writePos, ctr));
        return *this;
    }

    ByteBuffer& operator<<(const Next& next)
    {
        ++m_counters.top().count;
        return *this;
    }

    ByteBuffer& operator<<(const End& end)
    {
        auto& ctr = m_counters.top();

        switch (ctr.size)
        {
        case 1:  m_buffer[ctr.pos] = (byte) ctr.count; break;
        case 2:  at<ushort>(ctr.pos) = (ushort) ctr.count; break;
        default: at<uint>(ctr.pos) = ctr.count;
        }

        m_counters.pop();
        return *this;
    }

    template <typename T, int n>
    ByteBuffer& operator<<(const math::Vector<T, n>& vec)
    { return putPod(vec).inc(); }

    template <typename T>
    ByteBuffer& operator<<(const math::Matrix4x4<T>& mat)
    { return putPod(mat).inc(); }

protected:
    ByteBuffer(uint size = 0): m_buffer(size), m_readPos(0), m_writePos(0) {}
    ~ByteBuffer() {}

    void growToFit(uint size)
    {
        if (m_writePos + size > m_buffer.size())
            m_buffer.resize(m_writePos + size);
    }

    template <typename T>
    ByteBuffer& putPod(const T& val)
    {
        growToFit(sizeof(val));

        at<T>(m_writePos) = val;
        m_writePos += sizeof(T);

        return *this;
    }

    ByteBuffer& putBlock(const void* data, uint size)
    {
        growToFit(size);

        memcpy(&m_buffer[m_writePos], data, size);
        m_writePos += size;

        return *this;
    }

    template <typename CharT, typename LenT>
    ByteBuffer& putString(const CharT* str, LenT len)
    {
        assert(len <= std::numeric_limits<ushort>::max());
        return putPod(len).putBlock(str, len);
    }


    struct CounterImpl
    {
        template <typename T>
        CounterImpl(uint& wpos, const T& ctr):
            trigger(ctr.trigger),
            size(sizeof(typename T::type)),
            pos(wpos), count(0), puts(0)
        {
            wpos += size;
        }

        const uint trigger, size;
        uint pos, count, puts;
    };

    ByteBuffer& inc()
    {
        if (m_counters.size())
        {
            auto& ctr = m_counters.top();

            if (ctr.trigger)
                if (++ctr.puts % ctr.trigger == 0)
                    ++ctr.count;
        }
        return *this;
    }


    std::vector<byte> m_buffer;
    std::stack<CounterImpl> m_counters;
    uint m_readPos, m_writePos;
};

} // ns bb
} // ns mcr
