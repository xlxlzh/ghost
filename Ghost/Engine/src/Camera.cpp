#include "Camera.h"

namespace ghost
{
    void Camera::setViewport(const ViewportPtr& vPtr)
    {
        _viewport = vPtr;
    }

    void Camera::update()
    {
        _absPos = Vector3f(_absTrans._41, _absTrans._42, _absTrans._43);

        _matView = _absTrans.inverse();

        _matProj = Matrix4x4f::perspectiveMatrix(_fov, _aspect, _nearPlane, -_farPlane);
    }

    void Camera::setProjectParams(float fov, float aspect, float n, float f)
    {
        _fov = fov;
        _aspect = aspect;
        _nearPlane = n;
        _farPlane = f;
    }
}