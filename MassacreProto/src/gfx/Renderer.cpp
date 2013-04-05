#include "Universe.h"
#include "gfx/Renderer.h"

#include "gfx/Context.h"
#include "Camera.h"
#include "Object.h"
#include "gfx/renderables/Mesh.h"

using namespace mcr;
using namespace gfx;

RenderData::RenderData(int pass_, const Object* obj_, const RenderAtom* atom):
    pass(pass_), obj(obj_), mtl(atom->material), vao(atom->vao),
    offset(sizeof(uint) * atom->start), numIndices(atom->size) {}

bool gfx::operator<(const RenderData& lhs, const RenderData& rhs)
{
    if (lhs.pass != rhs.pass)
        return lhs.pass < rhs.pass;

    if (lhs.mtl != rhs.mtl)
        return lhs.mtl < rhs.mtl;

    if (lhs.mtl->renderStateHash() != rhs.mtl->renderStateHash())
        return lhs.mtl->renderStateHash() < rhs.mtl->renderStateHash();

    if (lhs.vao != rhs.vao)
        return lhs.vao < rhs.vao;

    if (lhs.offset != rhs.offset)
        return lhs.offset < rhs.offset;

    return lhs.numIndices < rhs.numIndices;
}

const RenderData* Renderer::registerData(int pass, const Object* obj, const RenderAtom* atom)
{
    auto ib = m_data.insert(RenderData(pass, obj, atom));
    return ib.second ? &*ib.first : nullptr;
}

const RenderData* Renderer::registerData(const Object* obj, const RenderAtom* atom)
{
    return registerData(atom->material->passHint(), obj, atom);
}


void Renderer::deregisterData(const RenderData* data)
{
    m_data.erase(*data);
}

void Renderer::deregisterData(int pass, const Object* obj, const RenderAtom* atom)
{
    RenderData data(pass, obj, atom);
    deregisterData(&data);
}

void Renderer::deregisterData(const Object* obj, const RenderAtom* atom)
{
    deregisterData(atom->material->passHint(), obj, atom);
}


void Renderer::render()
{
    auto& ctx = Context::active();

    uint vao = 0;

    // consider using BOOST_FOREACH, question overall iteration performance
    for (auto it = m_data.begin(); it != m_data.end(); ++it)
    {
        auto& data = *it;

        if (data.vao != vao)
        {
            glBindVertexArray(data.vao);
            vao = data.vao;
        }

        ctx.setActiveMaterial(data.mtl);
        glDrawElements(GL_TRIANGLES, data.numIndices, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(data.offset));
    }
}
