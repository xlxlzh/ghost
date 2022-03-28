#ifndef _MODELNODE_H_
#define _MODELNODE_H_

#include "SceneNode.h"
#include "Camera.h"
#include "Material.h"
#include "ConstBuffer.h"
#include "Model.h"

namespace ghost
{
    class GHOST_API ModelNode : public SceneNode
    {
    public:
        ModelNode(SceneManager* owner);
        ~ModelNode() { }

        void setModel(const ModelPtr& model);
        const ModelPtr& getMesh() const { return _mesh; }

        void prepareRendering(Camera* cam);

        void setMaterial(const MaterialPtr& mat) { _material = mat; }

        virtual void render(Camera* cam);

        void getRenderOperation(unsigned index, RenderOperation& op);

        GET_SCENENODE_TYPE(MODEL)

    protected:
        virtual void onPostUpdate();

    protected:
        ModelPtr _mesh = nullptr;

        MaterialPtr _material = nullptr;

        ConstBufferPtr _meshParams;

        BoundingBox _localBox;
    };
}

#endif
