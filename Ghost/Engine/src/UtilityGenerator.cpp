#include "UtilityGenerator.h"

namespace ghost
{
    ModelPtr UtilityGenerator::createBox(const Vector3f& size)
    {
        ModelPtr model = GHOST_MAKE_SMART_POINTER(Model);

        SubMesh subMesh{};

        float halfWidth = size._x / 2.0f;
        float halfHeight = size._y / 2.0f;
        float halfDepth = size._z / 2.0f;

        std::vector<float> datas =
        {
            -halfWidth, halfHeight, -halfDepth,

            halfWidth, halfHeight, -halfDepth,

            halfWidth, -halfHeight, -halfDepth,

            -halfWidth, -halfHeight, -halfDepth,

            -halfWidth, halfHeight, halfDepth,

            -halfWidth, -halfHeight, halfDepth,

            halfWidth, -halfHeight, halfDepth,

            halfWidth, halfHeight, halfDepth,
        };

        std::vector<unsigned> indices =
        {
            0, 1, 2
        };

        subMesh.setVertexDatas(datas);
        subMesh.setIndices(indices);

        subMesh.createVertexBinding(0);

        model->addSubmesh(subMesh);

        return model;
    }

    ModelPtr UtilityGenerator::createSphere(float radius)
    {
        ModelPtr model = GHOST_MAKE_SMART_POINTER(Model);
        return model;
    }
}