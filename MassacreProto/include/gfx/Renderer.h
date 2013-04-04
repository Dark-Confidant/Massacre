#pragma once

#include <set>
#include "MassacreTypes.h"

namespace mcr {

class Camera;
class Object;

namespace gfx {

class Material;
struct RenderAtom;

struct RenderData
{
    RenderData(int pass, const Object*, const RenderAtom*);

    int pass;
    const Object* obj;
    Material* mtl;

    uint vao;
    uint offset, numIndices;
};

bool operator<(const RenderData& lhs, const RenderData& rhs);

class Renderer
{
public:
    Renderer(Camera* cam): m_camera(cam) {}
    ~Renderer() {}

    Camera* camera() const { return m_camera; }
    void setCamera(Camera* cam) { m_camera = cam; }

    const RenderData* registerData(int pass, const Object* obj, const RenderAtom* atom);
    const RenderData* registerData(const Object* obj, const RenderAtom* atom);

    void deregisterData(const RenderData*);
    void deregisterData(int pass, const Object* obj, const RenderAtom* atom);
    void deregisterData(const Object* obj, const RenderAtom* atom);

    void render();

protected:
    Camera* m_camera;
    std::set<RenderData> m_data;
};

} // ns gfx
} // ns mcr
