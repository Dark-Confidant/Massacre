#pragma once

#include "ShaderProgram.h"
#include "Context.h"
#include "Channel.h"

namespace mcr {
namespace gfx {

template <typename T>
class UniformSlot
{
public:
    UniformSlot<T>(ShaderProgram* program, int location):
        m_program(program), m_location(location) {}

    UniformSlot<T>(ShaderProgram* program, const char* name):
        m_program(program)
    {
        m_location = m_program->findUniform(name);
    }

    void set(const T& value)
    {
        Context::active().setUniform(m_location, value);
    }

    OutputChannel<T> ch()
    {
        return outChannel<MCR_TYPEVAL(&UniformSlot<T>::set)>(this);
    }

private:
    ShaderProgram* m_program;
    int m_location;
};

} // ns gfx
} // ns mcr