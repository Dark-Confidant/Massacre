#include "Universe.h"
#include "GLState.h"

#include <mcr/Log.h>

namespace mcr {
namespace gfx {

static void GLAPIENTRY onGLDebug(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    GLvoid* userParam)
{
    (void) length;
    (void) userParam;

    static const char *const s_sourceLiterals[] =
    {
        "API", "Window System", "Shader Compiler",
        "Third Party", "Application", "Other"
    },
    *const s_typeLiterals[] =
    {
        "Error", "Deprecated Behavior", "Undefined Behavior",
        "Portability", "Performance", "Other"
    },
    *const s_severityLiterals[] = { "High", "Middle", "Low" };

    static const Log::Verbosity s_logLevelTable[] =
    {
        Log::Errors, Log::Warnings, Log::Warnings,
        Log::Info, Log::Debug, Log::Debug
    };

    source   -= GL_DEBUG_SOURCE_API_ARB;
    type     -= GL_DEBUG_TYPE_ERROR_ARB;
    severity -= GL_DEBUG_SEVERITY_HIGH_ARB;

    g_log->print(
        type     < arrsize(s_logLevelTable)    ? s_logLevelTable[type]        : Log::Errors,
        "[GL %s] [%s - %s severity] [%#010x]:\n%s",
        source   < arrsize(s_sourceLiterals)   ? s_sourceLiterals[source]     : "Unknown",
        type     < arrsize(s_typeLiterals)     ? s_typeLiterals[type]         : "Unknown",
        severity < arrsize(s_severityLiterals) ? s_severityLiterals[severity] : "Unknown",
        id, message);
}

GLState::GLState():
    m_activeTexUnit(0),
    m_activeVertexArray(0)
{
    glewExperimental = true;

    auto err = glewInit();
    if (err != GLEW_OK)
        g_log->error("glewInit() failed: %s", glewGetErrorString(err));

    if (GLEW_ARB_debug_output)
        glDebugMessageCallbackARB(onGLDebug, nullptr);

    glGetIntegerv(GL_VIEWPORT, &m_viewport[0][0]);

    GLint numTexUnits = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numTexUnits);

    m_texUnits.resize((std::size_t) numTexUnits);

    memset(m_buffers, 0, sizeof(m_buffers));

    GLint numUBOs = 0;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &numUBOs);
    m_buffersIndexed[UniformBuffer].resize((std::size_t) numUBOs);

    if (GLEW_ARB_transform_feedback3)
    {
        GLint numTFBOs = 0;
        glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &numTFBOs);
        m_buffersIndexed[TransformFeedbackBuffer].resize((std::size_t) numTFBOs);
    }
}

const irect& GLState::viewport() const
{
    return m_viewport;
}

void GLState::setViewport(const irect& vp)
{
    if (m_viewport == vp)
        return;

    glViewport(vp.left(), vp.bottom(), vp.width(), vp.height());
    m_viewport = vp;
}

uint GLState::activeTexUnit() const
{
    return m_activeTexUnit;
}

void GLState::setActiveTexUnit(uint unit)
{
    if (m_activeTexUnit == unit)
        return;

    glActiveTexture(GL_TEXTURE0 + unit);
    m_activeTexUnit = unit;
}

uint GLState::boundTexture(uint unit) const
{
    return m_texUnits[unit];
}

uint GLState::boundTexture() const
{
    return m_texUnits[m_activeTexUnit];
}

void GLState::bindTexture(uint tex)
{
    if (m_texUnits[m_activeTexUnit] == tex)
        return;

    glBindTexture(GL_TEXTURE_2D, tex);
    m_texUnits[m_activeTexUnit] = tex;
}

void GLState::bindTexture(uint unit, uint tex)
{
    if (m_texUnits[unit] == tex)
        return;

    setActiveTexUnit(unit);
    bindTexture(tex);
}

uint GLState::activeProgram() const
{
    return m_activeProgram;
}

void GLState::setActiveProgram(uint program)
{
    if (m_activeProgram == program)
        return;

    glUseProgram(program);
    m_activeProgram = program;
}

uint GLState::boundBuffer(uint target) const
{
    uint tindex = bufferTargetEnumToIndex(target);

    if (m_activeVertexArray && tindex >= ArrayBuffer && tindex <= ElementArrayBuffer)
        return m_vertexArrays[m_activeVertexArray].buffers[tindex - ArrayBuffer];

    return m_buffers[tindex];
}

uint GLState::boundBuffer(uint target, uint index) const
{
    return m_buffersIndexed[bufferTargetEnumToIndex(target)][index];
}

void GLState::bindBuffer(uint target, uint buffer)
{
    uint tindex = bufferTargetEnumToIndex(target);

    uint& binding = m_activeVertexArray
                  ? m_vertexArrays[m_activeVertexArray].buffers[tindex - ArrayBuffer]
                  : m_buffers[tindex];
    
    if (binding == buffer)
        return;

    glBindBuffer(target, buffer);
    binding = buffer;
}

void GLState::bindBufferBase(uint target, uint index, uint buffer)
{
    uint tindex = bufferTargetEnumToIndex(target);
    uint& binding = m_buffersIndexed[tindex][index];

    if (binding == buffer)
        return;

    glBindBufferBase(target, index, buffer);
    binding = buffer;
    m_buffers[tindex] = buffer;
}

uint GLState::boundVertexArray() const
{
    return m_activeVertexArray;
}

void GLState::bindVertexArray(uint va)
{
    if (m_activeVertexArray == va)
        return;

    glBindVertexArray(va);
    m_activeVertexArray = va;

    if (m_vertexArrays.size() <= va)
        m_vertexArrays.resize(va + 1);
}

uint GLState::bufferTargetEnumToIndex(uint target)
{
    switch (target)
    {
    case GL_UNIFORM_BUFFER:             return UniformBuffer;
    case GL_TRANSFORM_FEEDBACK_BUFFER:  return TransformFeedbackBuffer;
    //case GL_ATOMIC_COUNTER_BUFFER:      return AtomicCounterBuffer;
    //case GL_SHADER_STORAGE_BUFFER:      return ShaderStorageBuffer;
    case GL_ARRAY_BUFFER:               return ArrayBuffer;
    case GL_ELEMENT_ARRAY_BUFFER:       return ElementArrayBuffer;
    case GL_TEXTURE_BUFFER:             return TextureBuffer;
    //case GL_COPY_READ_BUFFER:           return CopyReadBuffer;
    //case GL_COPY_WRITE_BUFFER:          return CopyWriteBuffer;
    //case GL_DRAW_INDIRECT_BUFFER:       return DrawIndirectBuffer;
    //case GL_DISPATCH_INDIRECT_BUFFER:   return DispatchIndirectBuffer;
    //case GL_PIXEL_PACK_BUFFER:          return PixelPackBuffer;
    //case GL_PIXEL_UNPACK_BUFFER:        return PixelUnpackBuffer;
    }
    return NumBufferTargets;
}

void GLState::setRenderer(const char* renderer)
{
    m_renderer = renderer;
}

void GLState::setVendor(const char* vendor)
{
    m_vendor = vendor;
}

std::string GLState::renderer() const
{
    return m_renderer;
}

std::string GLState::vendor() const
{
    return m_vendor;
}

} // ns gfx
} // ns mcr
