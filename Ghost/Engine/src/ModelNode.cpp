#include "ModelNode.h"
#include "Engine.h"
#include "Model.h"
#include "ShaderConstBufferStruct.h"
#include "Light.h"
#include "SceneManager.h"

namespace ghost
{
    ModelNode::ModelNode(SceneManager* owner) : Renderable(owner)
    {

    }

    void ModelNode::SetModel(const ModelPtr& mesh)
    {
        _mesh = mesh;
        _dirty = true;
    }

    void ModelNode::SetInstanceMaterial(unsigned subindex, const MaterialPtr& mat)
    {

    }

    void ModelNode::PrepareRendering(Camera* cam, unsigned index)
    {
        if (_meshParams == nullptr)
            _meshParams = Engine::GetInstance()->GetRenderDevice()->CreateConstBuffer(sizeof(PerObject), ResourceUsage::USAGE_DYNAMIC, "PerObject");

        auto renderSystem = Engine::GetInstance()->GetRenderSystem();
        Light* mainLight = _owner->GetMainLigt();

        const MaterialPtr& mat = _mesh->GetMaterial(index);
        renderSystem->SetMaterial(mat);

        Matrix4x4f worldMat = _mesh->_meshes[index]._localMatrix * _absTrans;

        PerObject obj;
        obj._matWorld = worldMat;
        obj._matWorldInverseTranspose = worldMat.Inverse().GetTransposed();

        if (renderSystem->GetRenderPass() == RenderPass::RENDER_PASS_SHADOW)
            obj._matMVP = worldMat * _owner->GetShadowViewMat() * _owner->GetShadowProjMat();
        else
            obj._matMVP = worldMat * cam->GetViewMatrix() * cam->GetProjectMatrix();

        _meshParams->WriteData(0, sizeof(PerObject), &obj, true);
    }

    void ModelNode::Render(Camera* cam)
    {
        auto renderSystem = Engine::GetInstance()->GetRenderSystem();

        for (unsigned i = 0; i < _mesh->_meshes.size(); ++i)
        {
            PrepareRendering(cam, i);
            renderSystem->SetConstBuffer(SHADER_VS, _meshParams);

            RenderOperation op;
            GetRenderOperation(i, op);
            renderSystem->Render(op);
        }
    }

    void ModelNode::GetRenderOperation(unsigned index, RenderOperation& op)
    {
        op._indexBuffer = _mesh->_meshes[index]._indexBuffer;
        op._vertexBinding = _mesh->_meshes[index]._bindings;
        op._primitiveType = PRIMITIVE_TRIANGLELIST;
        op._vertexDecl = _mesh->_vertexDec;
    }

    void ModelNode::OnPostUpdate()
    {
        //Reset bounding box
        _localBox = BoundingBox();

        if (!_mesh)
            return;

        //Calculate new bounding box.
        /*const auto& vertices = _mesh->getVertices();

        Vector3f vMin(FLT_MAX, FLT_MAX, FLT_MAX);
        Vector3f vMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (const auto& v : vertices)
        {
            vMin._x = std::min(v._x, vMin._x);
            vMin._y = std::min(v._y, vMin._y);
            vMin._z = std::min(v._z, vMin._z);

            vMax._x = std::max(v._x, vMax._x);
            vMax._y = std::max(v._y, vMax._y);
            vMax._z = std::max(v._z, vMax._z);
        }

        _localBox.setExtents(vMin, vMax);

        //Transform bounding box to world space.
        _boundingBox = _localBox;
        _boundingBox.transform(_absTrans);

        for (const auto& child : _children)
            _boundingBox.merge(child->getBoundingBox());*/
    }
}