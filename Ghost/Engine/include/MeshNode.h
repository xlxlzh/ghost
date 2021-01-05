#ifndef _MESHNODE_H_
#define _MESHNODE_H_

#include "SceneNode.h"
#include "Camera.h"
#include "Material.h"

namespace ghost
{
    class Mesh;
    class GHOST_API MeshNode : public SceneNode
    {
    public:
        MeshNode(SceneManager* owner) : SceneNode(owner) { }
        ~MeshNode() { }

        void setMesh(Mesh* mesh) { _mesh = mesh; }
        const Mesh* getMesh() const { return _mesh; }

        void setMaterial(Material* mat) { _material = mat; }

        virtual void render(Camera* cam);

    protected:
        Mesh* _mesh = nullptr;

        Material* _material = nullptr;
    };
}

#endif
