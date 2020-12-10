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
            normal = Vector3f(a, b, c);
            float invLen = 1.0f / normal.length();
            normal *= invLen;
            dist = d * invLen;
        }
        Plane(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2)
        {
            normal = v1 - v0;
            normal = normal.crossProduct(v2 - v0);
            normal.normalize();
            dist = -normal.dotProduct(v0);
        }

        float distance(const Vector3f& v) const
        {
            return normal.dotProduct(v) + dist;
        }

    public:
        Vector3f normal;
        float dist;
    };
}

#endif
