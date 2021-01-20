#include "Frustum.h"

namespace ghost
{
    Frustum::Frustum()
    {

    }

    Frustum::~Frustum()
    {

    }

    void Frustum::buildViewFrustum(const Matrix4x4f& mat, float fov, float aspect, float nearPlane, float farPlane)
    {
        float ymax = nearPlane * MathUtilities::tan(fov / 2.0f);
        float xmax = ymax * aspect;

        buildViewFrustum(mat, -xmax, xmax, -ymax, ymax, nearPlane, farPlane);
    }

    void Frustum::buildViewFrustum(const Matrix4x4f& mat, float left, float right, float bottom, float top, float nearPlane, float farPlane)
    {
        float ratio = farPlane / nearPlane;
        float leftFar = left * ratio;
        float rightFar = right * ratio;
        float bottomFar = bottom * ratio;
        float topFar = top * ratio;

        //Get points on near plane
        _corners[0] = Vector3f(left, bottom, nearPlane);
        _corners[1] = Vector3f(left, top, nearPlane);
        _corners[2] = Vector3f(right, top, nearPlane);
        _corners[3] = Vector3f(right, bottom, nearPlane);

        //Get points on far plane
        _corners[4] = Vector3f(leftFar, bottomFar, farPlane);
        _corners[5] = Vector3f(leftFar, topFar, farPlane);
        _corners[6] = Vector3f(rightFar, topFar, farPlane);
        _corners[7] = Vector3f(rightFar, bottomFar, farPlane);

        //Transform points
        _origin = Vector3f(0.0f, 0.0f, 0.0f) * mat;
        for (auto& corner : _corners)
            corner = corner * mat;

        //Calculate planes
        _planes[0] = Plane(_origin, _corners[0], _corners[1]);   //Left plane
        _planes[1] = Plane(_origin, _corners[2], _corners[3]);   //Right plane
        _planes[2] = Plane(_origin, _corners[3], _corners[0]);   //Bottom plane
        _planes[3] = Plane(_origin, _corners[1], _corners[2]);   //Top plane
        _planes[4] = Plane(_corners[0], _corners[1], _corners[2]); //Near plane
        _planes[5] = Plane(_corners[5], _corners[4], _corners[7]); //Far plane
    }

    void Frustum::buildViewFrustum(const Matrix4x4f& viewMat, const Matrix4x4f& projMat)
    {
        //TODO
    }

    void Frustum::buildBoxFrustum(const Matrix4x4f& mat, float left, float right, float bottom, float top, float front, float back)
    {
        //Front plane points
        _corners[0] = Vector3f(left, bottom, front);
        _corners[1] = Vector3f(left, top, front);
        _corners[2] = Vector3f(right, top, front);
        _corners[3] = Vector3f(right, bottom, front);

        //Back plane points
        _corners[4] = Vector3f(left, bottom, back);
        _corners[5] = Vector3f(left, top, back);
        _corners[6] = Vector3f(right, top, back);
        _corners[7] = Vector3f(right, bottom, back);

        _origin = Vector3f(0.0f, 0.0f, 0.f) * mat;
        for (auto& corner : _corners)
            corner = corner * mat;

        //Calculate planes
        _planes[0] = Plane(_corners[0], _corners[4], _corners[5]);   //Left plane
        _planes[1] = Plane(_corners[3], _corners[2], _corners[6]);   //Right plane
        _planes[2] = Plane(_corners[0], _corners[3], _corners[7]);   //Bottom plane
        _planes[3] = Plane(_corners[2], _corners[1], _corners[5]);   //Top plane
        _planes[4] = Plane(_corners[0], _corners[1], _corners[2]); //Near plane
        _planes[5] = Plane(_corners[5], _corners[4], _corners[7]); //Far plane
    }

    bool Frustum::cullSphere(Vector3f pos, float rad) const
    {
        for (const auto& pl : _planes)
        {
            if (pl.distance(pos) > rad)
                return true;
        }

        return false;
    }

    bool Frustum::cullBox(BoundingBox& box) const 
    {
        for (const auto& pl : _planes)
        {
            const Vector3f& n = pl._normal;

            Vector3f positive = box._min;
            if (n._x <= 0)
                positive._x = box._max._x;
            if (n._y <= 0)
                positive._y = box._max._y;
            if (n._z <= 0)
                positive._z = box._max._z;

            if (pl.distance(positive) > 0)
                return true;
        }

        return false;
    }
}