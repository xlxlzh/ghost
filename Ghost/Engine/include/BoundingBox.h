#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include "Ghost.h"
#include "Vector3.h"

namespace ghost
{
    class GHOST_API BoundingBox
    {
    public:
        BoundingBox() = default;
        BoundingBox(const Vector3<float>& vMin, const Vector3<float>& vMax);
        BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
        ~BoundingBox() = default;

        Vector3<float> getSize() const { return _max - _min; }
        Vector3<float> getCenter() const;

        Vector3<float> getHalfSize() const;

        bool contains(const Vector3<float>& point) const;

    public:
        Vector3<float> _min;
        Vector3<float> _max;
    };
}

#endif
