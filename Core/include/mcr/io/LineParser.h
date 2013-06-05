#pragma once

#include <mcr/io/IReader.h>

namespace mcr {
namespace io  {

class LineParser
{
public:
    LineParser(IReader* reader): m_reader(reader) {}
    ~LineParser() {} // inherit not

    bool                readLine();

    const std::string&  line() const;
    std::size_t         indent() const;

private:
    rcptr<IReader>  m_reader;

    std::string     m_line;
    std::size_t     m_indent;
};


inline bool LineParser::readLine()
{
    // skip empty lines (including the fake ones between terminators)
    for (m_line.clear(); m_line.empty();)
    {
        if (!m_reader->readString(m_line, "\r\n"))
        {
            m_indent = 0;
            return false;
        }
    }

    auto firstNonSpace = m_line.find_first_not_of(" \xA0\t");

    m_indent = firstNonSpace != std::string::npos
             ? firstNonSpace
             : m_line.size();

    m_line = m_line.substr(m_indent);

    return true;
}

inline const std::string& LineParser::line() const
{
    return m_line;
}

inline std::size_t LineParser::indent() const
{
    return m_indent;
}

} // ns io
} // ns mcr
