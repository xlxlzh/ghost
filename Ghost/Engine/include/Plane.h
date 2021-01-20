#ifndef _PLANE_H_
#define _PLANE_H_

#include "Ghost.h"
#include "Vector3.h"

namespace ghost
{
    class GHOST_API Plane
    {
    public:
        Plane() = default;
        Plane(float a, float b, float c, float d)
        {
            _normal = Vector3f(a, b, c);
            float invLen = 1.0f / _normal.length();
            _normal *= invLen;
            _dist = d * invLen;
        }
        Plane(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2)
        {
            _normal = v1 - v0;
            _normal = _normal.crossProduct(v2 - v0);
            _normal.normalize();
            _dist = -_normal.dotProduct(v0);
        }

        float distance(const Vector3f& v) const
        {
            return _normal.dotProduct(v) + _dist;
        }

    public:
        Vector3f _normal;
        float _dist;
    };
}

#endif
