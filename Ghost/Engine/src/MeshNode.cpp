#include "MeshNode.h"
#include "Engine.h"
#include "Mesh.h"

namespace ghost
{
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

        renderSystem->setVertexDeclaration(_mesh->_vertexDec);
        renderSystem->drawPrimitiveIndexed(_mesh->_indexBuffer->getNumIndices(), 0, 0);
    }
}