#include "MeshNode.h"
#include "Engine.h"
#include "Mesh.h"
#include "ShaderConstBufferStruct.h"
#include "Light.h"
#include "SceneManager.h"

namespace ghost
{
    MeshNode::MeshNode(SceneManager* owner) : SceneNode(owner)
    {
        _renderable = true;
    }

    void MeshNode::setMesh(const MeshPtr& mesh)
    {
        _mesh = mesh;
        _dirty = true;
    }

    void MeshNode::prepareRendering(Camera* cam)
    {
        if (_meshParams == nullptr)
            _meshParams = Engine::getInstance()->getRenderDevice()->createConstBuffer(sizeof(PerObject), ResourceUsage::USAGE_DYNAMIC, "PerObject");

        auto renderSystem = Engine::getInstance()->getRenderSystem();
        Light* mainLight = _owner->getMainLigt();

        PerObject obj;
        obj._matWorld = _absTrans;
        obj._matWorldInverseTranspose = _absTrans.inverse().transpose();

        if (renderSystem->getRenderPass() == RenderPass::RENDER_PASS_SHADOW)
            ;// obj._matMVP = _absTrans * mainLight->getViewMatrix() * mainLight->getProjectMatrix();
        else
            obj._matMVP = _absTrans * cam->getViewMatrix() * cam->getProjectMatrix();

        _meshParams->writeData(0, sizeof(PerObject), &obj, true);
    }

    void MeshNode::render(Camera* cam)
    {
        auto renderSystem = Engine::getInstance()->getRenderSystem();

        if (_material)
        {
            renderSystem->setMaterial(_material);
        }

        prepareRendering(cam);
        renderSystem->setConstBuffer(SHADER_VS, _meshParams);

        RenderOperation op;
        getRenderOperation(op);
        renderSystem->render(op);
    }

    void MeshNode::getRenderOperation(RenderOperation& op)
    {
        op._indexBuffer = _mesh->_indexBuffer;
        op._vertexBinding = _mesh->_bindings;
        op._primitiveType = PRIMITIVE_TRIANGLELIST;
        op._vertexDecl = _mesh->_vertexDec;
    }

    void MeshNode::onPostUpdate()
    {
        //Reset bounding box
        _localBox = BoundingBox();

        if (!_mesh)
            return;

        //Calculate new bounding box.
        const auto& vertices = _mesh->getVertices();

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
            _boundingBox.merge(child->getBoundingBox());
    }
}