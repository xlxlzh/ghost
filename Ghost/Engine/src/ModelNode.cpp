#include "ModelNode.h"
#include "Engine.h"
#include "Model.h"
#include "ShaderConstBufferStruct.h"
#include "Light.h"
#include "SceneManager.h"

namespace ghost
{
    ModelNode::ModelNode(SceneManager* owner) : SceneNode(owner)
    {
        _renderable = true;
    }

    void ModelNode::setModel(const ModelPtr& mesh)
    {
        _mesh = mesh;
        _dirty = true;
    }

    void ModelNode::setInstanceMaterial(unsigned subindex, const MaterialPtr& mat)
    {

    }

    void ModelNode::prepareRendering(Camera* cam, unsigned index)
    {
        if (_meshParams == nullptr)
            _meshParams = Engine::getInstance()->getRenderDevice()->createConstBuffer(sizeof(PerObject), ResourceUsage::USAGE_DYNAMIC, "PerObject");

        auto renderSystem = Engine::getInstance()->getRenderSystem();
        //Light* mainLight = _owner->getMainLigt();

        const MaterialPtr& mat = _mesh->getMaterial(index);
        renderSystem->setMaterial(mat);

        Matrix4x4f worldMat = _mesh->_meshes[index]._localMatrix * _absTrans;

        PerObject obj;
        obj._matWorld = worldMat;
        obj._matWorldInverseTranspose = worldMat.inverse().getTransposed();

        if (renderSystem->getRenderPass() == RenderPass::RENDER_PASS_SHADOW)
            obj._matMVP = worldMat * cam->getViewMatrix() * cam->getProjectMatrix();
        else
            obj._matMVP = worldMat * cam->getViewMatrix() * cam->getProjectMatrix();

        _meshParams->writeData(0, sizeof(PerObject), &obj, true);
    }

    void ModelNode::render(Camera* cam)
    {
        auto renderSystem = Engine::getInstance()->getRenderSystem();

        for (unsigned i = 0; i < _mesh->_meshes.size(); ++i)
        {
            prepareRendering(cam, i);
            renderSystem->setConstBuffer(SHADER_VS, _meshParams);

            RenderOperation op;
            getRenderOperation(i, op);
            renderSystem->render(op);
        }
    }

    void ModelNode::getRenderOperation(unsigned index, RenderOperation& op)
    {
        op._indexBuffer = _mesh->_meshes[index]._indexBuffer;
        op._vertexBinding = _mesh->_meshes[index]._bindings;
        op._primitiveType = PRIMITIVE_TRIANGLELIST;
        op._vertexDecl = _mesh->_vertexDec;
    }

    void ModelNode::onPostUpdate()
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