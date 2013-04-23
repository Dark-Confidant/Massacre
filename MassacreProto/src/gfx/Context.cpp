#include "Universe.h"
#include "gfx/Context.h"

#if defined(MCR_PLATFORM_WINDOWS)
#   include <GL/wglew.h>
#elif defined(MCR_PLATFORM_LINUX)
#   include <GL/glxew.h>
#endif

#include "gfx/ShaderProgram.h"
#include "gfx/Texture.h"
#include "gfx/Material.h"
#include "gfx/GBuffer.h"

#include "gfx/experimental/Mesh.h"
#include "gfx/experimental/MeshStorage.h"

#include "GLEnums.inl"

namespace mcr {
namespace gfx {

Context* Context::s_active;

Context::Context():
    m_activeProgram(),
    m_activeTextureUnit(0),
    m_nextFreeTextureUnit(0),
    m_nextFreeBufferUnit(0),
    m_activeVertexArray()
{
    if (!s_active)
        s_active = this;

    GLint maxTexUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnits);

    TextureUnit refUnit = {};
    m_textureUnits.resize((size_t) maxTexUnits, refUnit);

    memset(m_buffers, 0, sizeof(m_buffers));

    for (int i = 0; i < GBuffer::NumTypes; ++i)
    {
        GLint maxUnits = 0;

        switch (i)
        {
        case GBuffer::UniformBuffer:
            glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUnits);
            break;
        }

        if (maxUnits)
            m_bufferUnits[i].resize((size_t) maxUnits);
    }

    // HACK!
    glGetIntegerv(GL_VIEWPORT, &m_viewport[0][0]);

    GLboolean writeMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &writeMask);
    m_renderState.depthWrite    = GL_TRUE == writeMask;
    m_renderState.depthTest     = GL_TRUE == glIsEnabled(GL_BLEND);
    m_renderState.alphaTest     = GL_TRUE == glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE);
    m_renderState.blend         = GL_TRUE == glIsEnabled(GL_BLEND);
    m_renderState.cullFace      = GL_TRUE == glIsEnabled(GL_CULL_FACE);
    m_renderState.polygonOffset = GL_TRUE == glIsEnabled(GL_POLYGON_OFFSET_FILL);

    GLint depthFn, blendFnSrc, blendFnDst;
    glGetIntegerv(GL_DEPTH_FUNC, &depthFn);
    glGetIntegerv(GL_BLEND_SRC,  &blendFnSrc);
    glGetIntegerv(GL_BLEND_DST,  &blendFnDst);

    for (uint i = 0; i < DepthFn::NumFns; ++i)
    {
        if (g_depthFnTable[i] == (GLenum) depthFn)
        {
            m_renderState.depthFunc = DepthFn(i);
            break;
        }
    }

    for (uint i = 0; i < BlendFn::NumFactors; ++i)
    {
        if (g_blendFnTable[i] == (GLenum) blendFnSrc)
            m_renderState.blendFunc.srcFactor = BlendFn::Factor(i);

        if (g_blendFnTable[i] == (GLenum) blendFnDst)
            m_renderState.blendFunc.dstFactor = BlendFn::Factor(i);
    }

    m_renderStateHash = m_renderState.hash();

#ifdef MCR_PLATFORM_WINDOWS
    wglSwapIntervalEXT(0);
#endif
}

Context::~Context()
{
    if (s_active == this)
        s_active = nullptr;
}

bool Context::activate()
{
    //wglMakeCurrent()
    s_active = this;
    return true;
}

void Context::setViewport(const irect& viewport)
{
    if (m_viewport == viewport)
        return;

    glViewport(viewport.left(), viewport.bottom(), viewport.width(), viewport.height());
    m_viewport = viewport;
}

void Context::setRenderState(const RenderState& rs)
{
    if (m_renderState.depthTest != rs.depthTest)
        (rs.depthTest ? glEnable : glDisable)(GL_DEPTH_TEST);

    if (rs.depthTest)
    {
        if (m_renderState.depthFunc != rs.depthFunc)
            glDepthFunc(g_depthFnTable[rs.depthFunc.fn]);

        if (m_renderState.depthWrite != rs.depthWrite)
            glDepthMask(rs.depthWrite);
    }

    if (m_renderState.blend != rs.blend)
        (rs.depthTest ? glEnable : glDisable)(GL_BLEND);

    if (rs.blend && m_renderState.blendFunc != rs.blendFunc)
    {
        glBlendFunc(g_blendFnTable[rs.blendFunc.srcFactor],
                    g_blendFnTable[rs.blendFunc.dstFactor]);
    }

    if (m_renderState.alphaTest != rs.alphaTest)
        (rs.alphaTest ? glEnable : glDisable)(GL_SAMPLE_ALPHA_TO_COVERAGE);

    if (m_renderState.cullFace != rs.cullFace)
        (rs.cullFace ? glEnable : glDisable)(GL_CULL_FACE);

    if (m_renderState.polygonOffset != rs.polygonOffset)
        (rs.polygonOffset ? glEnable : glDisable)(GL_POLYGON_OFFSET_FILL);

    m_renderState     = rs;
    m_renderStateHash = rs.hash();
}

void Context::setActiveProgram(ShaderProgram* program)
{
    if (m_activeProgram == program)
        return;

    glUseProgram(program->handle());
    m_activeProgram = program;
}

void Context::setActiveTextureUnit(uint texUnit)
{
    if (m_activeTextureUnit == texUnit)
        return;

    glActiveTexture(GL_TEXTURE0 + texUnit);
    m_activeTextureUnit = texUnit;
}

void Context::bindTexture(TexTarget target, Texture* tex)
{
    if (m_textureUnits[m_activeTextureUnit].textures[target] == tex)
        return;

    static const GLenum targetTr[NumTexTargets] =
    {
        GL_TEXTURE_1D,
        GL_TEXTURE_2D,
        GL_TEXTURE_3D,
        GL_TEXTURE_CUBE_MAP
    };

    glBindTexture(targetTr[target], tex ? tex->handle() : 0);
    m_textureUnits[m_activeTextureUnit].textures[target] = tex;
}

uint Context::allocTextureUnit(uint* refs)
{
    auto& unit = m_textureUnits[m_nextFreeTextureUnit];

    ++unit.refs;

    if (refs)
        *refs = unit.refs;

    auto result = m_nextFreeTextureUnit;
    m_nextFreeTextureUnit = (m_nextFreeTextureUnit + 1) % (int) m_textureUnits.size();

    return result;
}

void Context::freeTextureUnit(uint texUnit) {}

void Context::setActiveMaterial(Material* mtl)
{
    if (m_activeMaterial == mtl)
        return;

    if (m_renderStateHash != mtl->renderStateHash())
        setRenderState(mtl->renderState());

    setActiveProgram(mtl->program());

    auto numBoundTextures = std::min<int>(
            m_activeProgram->numSamplers(),
            mtl->numTextures());

    for (int i = 0; i < numBoundTextures; ++i)
    {
        auto& sampler = m_activeProgram->sampler(i);
        auto  tex     = mtl->texture(i);

        if (activeTexture(TexTarget(sampler.target), sampler.unit) != tex)
        {
            setActiveTextureUnit(sampler.unit);
            bindTexture(TexTarget(sampler.target), tex);
        }
    }

    m_activeMaterial = mtl;
}

void Context::bindBuffer(GBuffer* buffer)
{
    if (!buffer)
        return;

    auto& slot = m_buffers[buffer->type()];

    if (slot == buffer)
        return;

    glBindBuffer(g_bufferTypeTable[buffer->type()], buffer->handle());
    slot = buffer;
}


void Context::bindBufferBase(uint bufUnit, GBuffer* buffer, uint offset)
{
    /*if (m_bufferUnits[target][bufUnit] == buffer)
        return bindBuffer;*/

    throw 1;
}

void Context::bindBufferRange(uint bufUnit, GBuffer* buffer, uint offset, uint count)
{
    throw 1;
}

void Context::setActiveVertexArray(VertexArray* va)
{
    if (m_activeVertexArray == va)
        return;

    glBindVertexArray(va ? va->handle() : 0);
    m_activeVertexArray = va;
}

void Context::clear()
{
    if (!m_renderState.depthWrite)
    {
        m_renderState.depthWrite = true;
        glDepthMask(true);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

experimental::MeshStorage* Context::activeMeshStorage() const
{
    throw 1;
    return nullptr;
}

void Context::setActiveMeshStorage(experimental::MeshStorage* storage)
{
    throw 1;
}

void Context::drawMesh(const experimental::Mesh& mesh)
{
    //setActiveMeshStorage(mesh.storage);
    setActiveVertexArray(mesh.buffer);

    glDrawElements(mesh.primitiveType, mesh.numIndices, GL_UNSIGNED_INT,
        reinterpret_cast<const GLvoid*>(mesh.startIndex));
}

} // ns gfx
} // ns mcr
