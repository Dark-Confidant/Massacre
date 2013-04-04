#include "Universe.h"
#include "gfx/RenderState.h"

mcr::gfx::RenderState::RenderState():
    alphaTest(false),
    depthTest(true),
    depthWrite(true),
    depthFunc(GL_LEQUAL),
    blend(false),
    blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
    cullFace(true),
    polygonOffset(false) {}
