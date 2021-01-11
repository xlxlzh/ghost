#include "Camera.h"
#include "Engine.h"
#include "ShaderConstBufferStruct.h"

namespace ghost
{
    void Camera::setViewport(const ViewportPtr& vPtr)
    {
        _viewport = vPtr;
    }

    void Camera::onPostUpdate()
    {
        _absPos = Vector3f(_absTrans._41, _absTrans._42, _absTrans._43);

        _matView = _absTrans.inverse();

        _matProj = Matrix4x4f::perspectiveMatrix(_fov, _aspect, _nearPlane, _farPlane);
    }

    void Camera::setProjectParams(float fov, float aspect, float n, float f)
    {
        _fov = fov;
        _aspect = aspect;
        _nearPlane = n;
        _farPlane = f;
    }

    void Camera::prepareForRendering()
    {
        if (_cameraParams == nullptr)
            _cameraParams = Engine::getInstance()->getRenderDevice()->createConstBuffer(sizeof(PerFrame), BufferUsage::USAGE_DYNAMIC, "PerFrame");

        PerFrame frame;
        frame._cameraPos = Vector4f(_absPos._x, _absPos._y, _absPos._z, 1.0f);

        _cameraParams->writeData(0, sizeof(PerFrame), &frame, true);
    }
}