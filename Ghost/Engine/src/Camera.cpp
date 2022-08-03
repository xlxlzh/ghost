#include "Camera.h"
#include "Engine.h"
#include "ShaderConstBufferStruct.h"
#include "MathUtilities.h"

namespace ghost
{
    void Camera::setViewport(const ViewportPtr& vPtr)
    {
        _viewport = vPtr;
    }

    void Camera::onPostUpdate()
    {
        _absPos = _pos;
        _absTrans._41 = _absPos._x;
        _absTrans._42 = _absPos._y;
        _absTrans._43 = _absPos._z;

        //_absPos = Vector3f(_absTrans._41, _absTrans._42, _absTrans._43);

        _matView = _absTrans.inverse();

        if (!_orthographic)
        {
            _matProj = Matrix4x4f::perspectiveMatrix(_fov, _aspect, _frustNear, _frustFar);
            _frustum.buildViewFrustum(_absTrans, _fov, _aspect, _frustNear, _frustFar);
        }   
        else
        {
            _matProj = Matrix4x4f::orthoMatrix(_frustRight - _frustLeft, _frustTop - _frustBottom, _frustNear, _frustFar);
            //_frustum.buildBoxFrustum();
        }
    }

    void Camera::setProjectParams(float fov, float aspect, float n, float f)
    {
        _fov = fov;
        _aspect = aspect;

        float ymax = n * MathUtilities::tan(fov / 2.0f);
        float xmax = ymax * aspect;

        _frustNear = n;
        _frustFar = f;
        _frustLeft = -xmax;
        _frustRight = xmax;
        _frustTop = ymax;
        _frustBottom = -ymax;

        markDirty();
    }

    void Camera::prepareForRendering()
    {
        if (_cameraParams == nullptr)
            _cameraParams = Engine::getInstance()->getRenderDevice()->createConstBuffer(sizeof(PerFrame), ResourceUsage::USAGE_DYNAMIC, "PerFrame");

        PerFrame frame;
        frame._cameraPos = Vector4f(_absPos._x, _absPos._y, _absPos._z, 1.0f);

        _cameraParams->writeData(0, sizeof(PerFrame), &frame, true);
    }
}