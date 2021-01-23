#include "BoundingBox.h"
#include "Vector4.h"

#include <algorithm>

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
        return (_max - _min) * 0.5f;
    }

    Vector3<float> BoundingBox::getCenter() const
    {
        return (_max + _min) * 0.5f;
    }

    bool BoundingBox::contains(const Vector3<float>& point) const
    {
        return point._x >= _min._x && point._y >= _min._y &&
            point._z >= _min._z && point._x <= _max._x && point._y <= _max._y &&
            point._z <= _max._z;
    }

    void BoundingBox::merge(const BoundingBox& rhs)
    {
        _min._x = std::min(_min._x, rhs._min._x);
        _min._y = std::min(_min._y, rhs._min._y);
        _min._z = std::min(_min._z, rhs._min._z);

        _max._x = std::max(_max._x, rhs._max._x);
        _max._y = std::max(_max._y, rhs._max._y);
        _max._z = std::max(_max._z, rhs._max._z);
    }

    void BoundingBox::transform(const Matrix4x4f& mat)
    {
        Vector3f vMin = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
        Vector3f vMax = Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (unsigned i = 0; i < 8; ++i)
        {
            Vector3f v = getCorner(i) * mat;

            vMin._x = std::min(v._x, vMin._x);
            vMin._y = std::min(v._y, vMin._y);
            vMin._z = std::min(v._z, vMin._z);

            vMax._x = std::max(v._x, vMax._x);
            vMax._y = std::max(v._y, vMax._y);
            vMax._z = std::max(v._z, vMax._z);
        }

        _min = vMin;
        _max = vMax;
    }

    Vector3f BoundingBox::getCorner(unsigned index) const
    {
        switch (index)
        {
        case 0:
            return Vector3f(_min._x, _min._y, _max._z);
        case 1:
            return Vector3f(_min._x, _max._y, _max._z);
        case 2:
            return Vector3f(_max._x, _max._y, _max._z);
        case 3:
            return Vector3f(_max._x, _min._y, _max._z);
        case 4:
            return Vector3f(_min._x, _min._y, _min._z);
        case 5:
            return Vector3f(_min._x, _max._y, _min._z);
        case 6:
            return Vector3f(_max._x, _max._y, _min._z);
        case 7:
            return Vector3f(_max._x, _min._y, _min._z);
        }

        return Vector3f();
    }
}