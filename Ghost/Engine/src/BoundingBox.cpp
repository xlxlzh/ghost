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

    Vector3<float> BoundingBox::getHalfSize() const
    {
        return (_max - _min) * 0.5;
    }

    Vector3<float> BoundingBox::getCenter() const
    {
        return (_max + _min) * 0.5;
    }

    bool BoundingBox::contains(const Vector3<float>& point) const
    {
        return point._x >= _min._x && point._y >= _min._y &&
            point._z >= _min._z && point._x <= _max._x && point._y <= _max._y &&
            point._z <= _max._z;
    }
}