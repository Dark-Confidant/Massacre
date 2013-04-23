#include "Universe.h"
#include <mcr/gfx/Material.h>

namespace mcr {
namespace gfx {

rcptr<Material> Material::create(byte numTex)
{
    auto result = new Material;
    result->setNumTextures(numTex);

    return result;
}

Material::Material(): m_passHint(0) { m_renderStateHash = m_renderState.hash(); }
Material::~Material() {}

} // ns gfx
} // ns mcr
