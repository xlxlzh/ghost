#include "MeshNode.h"
#include "Engine.h"
#include "Mesh.h"
#include "ShaderConstBufferStruct.h"

namespace ghost
{
    void MeshNode::prepareRendering(Camera* cam)
    {
        if (_meshParams == nullptr)
            _meshParams = Engine::getInstance()->getRenderDevice()->createConstBuffer(sizeof(PerObject), BufferUsage::USAGE_DYNAMIC, "PerObject");

        PerObject obj;
        obj._matMVP = _absTrans * cam->getViewMatrix() * cam->getProjectMatrix();
        obj._matWorld = _absTrans;
        obj._matWorldInverseTranspose = _absTrans.inverse().transpose();

        _meshParams->writeData(0, sizeof(PerObject), &obj, true);
    }

    void MeshNode::render(Camera* cam)
    {
        auto renderSystem = Engine::getInstance()->getRenderSystem();
        renderSystem->setVertexBufferBinding(_mesh->_bindings.get());
        renderSystem->setIndexBuffer(_mesh->_indexBuffer);
        renderSystem->setPrimitiveType(PRIMITIVE_TRIANGLELIST);

        if (_material)
        {
            renderSystem->setMaterial(_material);
        }

        prepareRendering(cam);
        renderSystem->setConstBuffer(SHADER_VS, _meshParams);

        renderSystem->setVertexDeclaration(_mesh->_vertexDec);
        renderSystem->drawPrimitiveIndexed(_mesh->_indexBuffer->getNumIndices(), 0, 0);
    }
}