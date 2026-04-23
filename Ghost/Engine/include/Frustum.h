#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "Ghost.h"
#include "Plane.h"
#include "BoundingBox.h"

#include <array>

namespace ghost
{
    constexpr unsigned FRUSTUM_CORNER_NUM = 8;
    constexpr unsigned FRUSTUM_PLANE_NUM = 6;

    using FrustumCorners = std::array<Vector3f, FRUSTUM_CORNER_NUM>;
    using FrustumPlanes = std::array<Plane, FRUSTUM_PLANE_NUM>;

    class GHOST_API Frustum
    {
    public:
        Frustum();
        ~Frustum();

        void BuildViewFrustum(const Matrix4x4f& mat, float fov, float aspect, float nearPlane, float farPlane);
        void BuildViewFrustum(const Matrix4x4f& mat, float left, float right, float bottom, float top, float nearPlane, float farPlane);
        void BuildViewFrustum(const Matrix4x4f& viewMat, const Matrix4x4f& projMat);

        void BuildBoxFrustum(const Matrix4x4f& mat, float left, float right, float bottom, float top, float front, float back);

        bool CullBox(BoundingBox& box) const;
        bool CullSphere(Vector3f pos, float rad) const;

        const FrustumCorners& GetFrustumCorners() const { return _corners; }

    private:
        FrustumPlanes _planes;
        FrustumCorners _corners;

        Vector3f _origin;
    };
}

#endif
