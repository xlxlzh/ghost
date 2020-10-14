#include "BoundingBox.h"

namespace ghost
{
    BoundingBox::BoundingBox(const Vector3<float>& vMin, const Vector3<float>& vMax)
        : _min(vMin), _max(vMax)
    {

    }

    BoundingBox::BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
    {
        _max._x = maxX;
        _max._y = maxY;
        _max._z = maxZ;

        _min._x = minX;
        _min._y = minY;
        _min._z = minZ;
    }
}