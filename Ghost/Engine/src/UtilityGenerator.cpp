#include "UtilityGenerator.h"

namespace ghost
{
    ModelPtr UtilityGenerator::CreateBox(const Vector3f& size)
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

        subMesh.SetVertexDatas(datas);
        subMesh.SetIndices(indices);

        subMesh.CreateVertexBinding(0);

        model->AddSubmesh(subMesh);

        return model;
    }

    ModelPtr UtilityGenerator::CreateSphere(float radius)
    {
        ModelPtr model = GHOST_MAKE_SMART_POINTER(Model);
        return model;
    }
}