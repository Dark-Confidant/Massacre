#pragma once

#include <vector>
#include <mcr/FileSystem.h>
#include <mcr/gfx/VertexArray.h>
#include <mcr/gfx/renderables/Renderable.h>

namespace mcr {
namespace gfx {

class Mesh: public Renderable
{
public:
    //! Create empty mesh
    static rcptr<Mesh> create();

    //! Create mesh from contents of \c file
    static rcptr<Mesh> createFromFile(IFile* file);


    //! Load mesh from \c file
    void load(IFile* file);


    //! Vertex and index buffers
    const VertexArray* buffer() const { return m_buffer; }


    //! Number of mesh parts
    uint numAtoms() const { return (uint) m_parts.size(); }

    //! Mesh part by index
    const RenderAtom* atom(uint idx) const { return &m_parts[idx]; }


    void optimizeAtomsTMP();

protected:
    Mesh() { m_buffer = VertexArray::create(); }
    ~Mesh() {}

    Mesh(const Mesh&);
    Mesh& operator=(const Mesh&);

    rcptr<VertexArray> m_buffer;
    std::vector<RenderAtom> m_parts;
};

} // ns gfx
} // ns mcr
