#pragma once

#include "ComRegistry.h"
#include "gfx/renderables/Mesh.h"

namespace mcr {

class Model: public Component
{
public:
    static ComDesc Desc;

    static rcptr<Model> create(Object* obj) { return new Model(obj); }

    static void enumFields(FieldTree& fields)
    {
        fields.add("file", &Model::meshFile, &Model::setMeshFile);
    }


    gfx::Mesh* mesh() const { return m_mesh; }
    void setMesh(gfx::Mesh* mesh);

    const char* meshFile() const { return ""; }
    void setMeshFile(const char* /*file*/) {}

protected:
    Model(Object* obj): Component(obj) {}
    ~Model() {}

    void onEvent(EventId event, const EventParams& params);

    rcptr<gfx::Mesh> m_mesh;
};

}
