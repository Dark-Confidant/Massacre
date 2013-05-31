#pragma once

#include <cstring>
#include <algorithm>
#include <streambuf>
#include <vector>
#include <mcr/NonCopyable.h>
#include <mcr/io/IReader.h>

namespace mcr {
namespace io  {

class InputStream: public std::streambuf, NonCopyable
{
public:
    InputStream(IReader* reader, size_t bufSize = 1024, size_t putSize = 16):
        m_reader(reader),
        m_putSize(std::max<size_t>(putSize, 1)),
        m_buffer(std::max(bufSize, m_putSize) + m_putSize)
    {
        auto end = &m_buffer.front() + m_buffer.size();
        setg(end, end, end);
    }

private:
    int_type underflow()
    {
        if (gptr() >= egptr())
        {
            auto base = &m_buffer.front(), start = base;

            if (base == eback())
            {
                std::memmove(base, egptr() - m_putSize, m_putSize);
                start += m_putSize;
            }

            auto bytes = m_reader->read(start, m_buffer.size() - (start - base));
            if (!bytes)
                return traits_type::eof();

            setg(base, start, start + bytes);
        }

        return traits_type::to_int_type(*gptr());
    }

    const rcptr<IReader>    m_reader;
    std::size_t             m_putSize;
    std::vector<char>       m_buffer;
};

} // ns io
} // ns mcr
