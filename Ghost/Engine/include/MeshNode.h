#ifndef _MESHNODE_H_
#define _MESHNODE_H_

#include "SceneNode.h"
#include "Camera.h"
#include "Material.h"
#include "ConstBuffer.h"
#include "Mesh.h"

namespace ghost
{
    class GHOST_API MeshNode : public SceneNode
    {
    public:
        MeshNode(SceneManager* owner);
        ~MeshNode() { }

        void setMesh(const MeshPtr& mesh);
        const MeshPtr& getMesh() const { return _mesh; }

        void prepareRendering(Camera* cam);

        void setMaterial(const MaterialPtr& mat) { _material = mat; }

        virtual void render(Camera* cam);

        GET_SCENENODE_TYPE(MESH)

    protected:
        virtual void onPostUpdate();

    protected:
        MeshPtr _mesh = nullptr;

        MaterialPtr _material = nullptr;

        ConstBufferPtr _meshParams;

        BoundingBox _localBox;
    };
}

#endif
