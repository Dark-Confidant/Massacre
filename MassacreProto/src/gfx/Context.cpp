#include "Universe.h"
#include <mcr/gfx/Context.h>

#if defined(MCR_PLATFORM_WINDOWS)
#   include <GL/wglew.h>
#elif defined(MCR_PLATFORM_LINUX)
#   include <GL/glxew.h>
#endif

#include <mcr/gfx/Texture.h>
#include <mcr/gfx/Material.h>
#include <mcr/gfx/Mesh.h>

#include "GLEnums.inl"

namespace mcr {
namespace gfx {

Context* Context::s_active;

Context::Context():
    m_activeTextureUnit(0),
    m_nextFreeTextureUnit(0),
    m_activeVertexArray()
{
    if (!s_active)
        s_active = this;

    GLint maxTexUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnits);

    m_textureUnits.resize((size_t) maxTexUnits, nullptr);

    memset(m_buffers, 0, sizeof(m_buffers));

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

void Context::setActiveTextureUnit(uint texUnit)
{
    if (m_activeTextureUnit == texUnit)
        return;

    glActiveTexture(GL_TEXTURE0 + texUnit);
    m_activeTextureUnit = texUnit;
}

void Context::bindTexture(Texture* tex)
{
    if (m_textureUnits[m_activeTextureUnit] == tex)
        return;

    glBindTexture(GL_TEXTURE_2D, tex ? tex->handle() : 0);
    m_textureUnits[m_activeTextureUnit] = tex;
}

void Context::setActiveMaterial(Material* mtl)
{
    if (m_activeMaterial == mtl)
        return;

    if (m_renderStateHash != mtl->renderStateHash())
        setRenderState(mtl->renderState());

    glUseProgram(mtl->programTMP());
    mtl->syncParameters();

    for (int i = 0; i < mtl->numTextures(); ++i)
    {
        auto unit = mtl->samplerTMP(i);
        auto tex  = mtl->texture(i);

        if (activeTexture(unit) != tex)
        {
            setActiveTextureUnit(unit);
            bindTexture(tex);
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

void Context::drawMesh(const Mesh& mesh)
{
    setActiveVertexArray(mesh.buffer);

    glDrawElements(g_primitiveTypeTable[mesh.primitiveType], mesh.numIndices,
        GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(mesh.startIndex << 2));
}

} // ns gfx
} // ns mcr
