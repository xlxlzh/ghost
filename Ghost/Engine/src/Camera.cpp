#include "Camera.h"
#include "Engine.h"
#include "ShaderConstBufferStruct.h"
#include "MathUtilities.h"

namespace ghost
{
    void Camera::SetViewport(const ViewportPtr& vPtr)
    {
        _viewport = vPtr;
    }

    void Camera::OnPostUpdate()
    {
        _absPos = Vector3f(_absTrans._41, _absTrans._42, _absTrans._43);

        _matView = _absTrans.Inverse();

        if (!_orthographic)
        {
            _matProj = Matrix4x4f::PerspectiveMatrix(_fov, _aspect, _frustNear, _frustFar);
            _frustum.BuildViewFrustum(_absTrans, _fov, _aspect, _frustNear, _frustFar);
        }   
        else
        {
            _matProj = Matrix4x4f::OrthoMatrix(_frustRight - _frustLeft, _frustTop - _frustBottom, _frustNear, _frustFar);
            //_frustum.buildBoxFrustum();
        }
    }

    void Camera::SetProjectParams(float fov, float aspect, float n, float f)
    {
        _fov = fov;
        _aspect = aspect;

        float ymax = n * MathUtilities::Tan(fov / 2.0f);
        float xmax = ymax * aspect;

        _frustNear = n;
        _frustFar = f;
        _frustLeft = -xmax;
        _frustRight = xmax;
        _frustTop = ymax;
        _frustBottom = -ymax;

        MarkDirty();
    }

    void Camera::PrepareForRendering()
    {
        if (_cameraParams == nullptr)
            _cameraParams = Engine::GetInstance()->GetRenderDevice()->CreateConstBuffer(sizeof(PerFrame), ResourceUsage::USAGE_DYNAMIC, "PerFrame");

        PerFrame frame;
        frame._cameraPos = Vector4f(_absPos._x, _absPos._y, _absPos._z, 1.0f);

        _cameraParams->WriteData(0, sizeof(PerFrame), &frame, true);
    }
}