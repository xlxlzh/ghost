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

        void SetModel(const ModelPtr& model);
        const ModelPtr& GetMesh() const { return _mesh; }

        void PrepareRendering(Camera* cam, unsigned index);

        void SetInstanceMaterial(unsigned subindex, const MaterialPtr& mat);

        virtual void Render(Camera* cam);

        void GetRenderOperation(unsigned index, RenderOperation& op);

        GET_SCENENODE_TYPE(MODEL)

    protected:
        virtual void OnPostUpdate();

    protected:
        ModelPtr _mesh = nullptr;

        std::vector<MaterialPtr> _instanceMaterial;

        ConstBufferPtr _meshParams;

        BoundingBox _localBox;
    };
}

#endif
