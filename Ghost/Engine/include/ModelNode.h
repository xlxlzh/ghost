#ifndef _MODELNODE_H_
#define _MODELNODE_H_

#include "Renderable.h"
#include "Camera.h"
#include "Material.h"
#include "ConstBuffer.h"
#include "Model.h"

namespace ghost
{
    class GHOST_API ModelNode : public Renderable
    {
    public:
        ModelNode(SceneManager* owner);
        ~ModelNode() { }

        void setModel(const ModelPtr& model);
        const ModelPtr& getMesh() const { return _mesh; }

        void prepareRendering(Camera* cam, unsigned index);

        void setInstanceMaterial(unsigned subindex, const MaterialPtr& mat);

        virtual void render(Camera* cam);

        void getRenderOperation(unsigned index, RenderOperation& op);

        GET_SCENENODE_TYPE(MODEL)

    protected:
        virtual void onPostUpdate();

    protected:
        ModelPtr _mesh = nullptr;

        std::vector<MaterialPtr> _instanceMaterial;

        ConstBufferPtr _meshParams;

        BoundingBox _localBox;
    };
}

#endif
