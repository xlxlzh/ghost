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

        Vector3<float> GetSize() const { return _max - _min; }
        Vector3<float> GetCenter() const;

        Vector3<float> GetHalfSize() const;

        bool Contains(const Vector3<float>& point) const;

        void SetExtents(const Vector3f& minV, const Vector3f& maxV) { _min = minV; _max = maxV; }

        void Merge(const BoundingBox& rhs);
        void Transform(const Matrix4x4f& mat);

        Vector3f GetCorner(unsigned index) const;

    public:
        Vector3<float> _min;
        Vector3<float> _max;
    };
}

#endif
