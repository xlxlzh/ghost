#include "Light.h"
#include "Engine.h"
#include "ShaderConstBufferStruct.h"

namespace ghost
{
    Light::Light(SceneManager* owner) : SceneNode(owner),
        _radius(0.0f), _falloff(1.0f), _spotInner(0.0f),
        _spotOuter(0.0f), _shiness(0.0f)
    {
        _lightColor = Color();
        _lightDir = Vector3f(0.0f, 0.0f, 1.0f);
        _position = Vector3f(0.0f, 0.0f, 0.0f);

        _viewMat.Identify();
    }

    Light::~Light()
    {

    }

    Vector3f Light::GetLightDir() const
    {
        if (_lightType != LIGHT_POINT)
            return _lightDir;
        else
            return _position;
    }

    void Light::OnPostUpdate()
    {
        _viewMat = _absTrans.Inverse();

        Matrix4x4f m = _absTrans;
        m._41 = 0.0f;
        m._42 = 0.0f;
        m._43 = 0.0f;

        _lightDir = Vector3f(0.0, 0.0, 1.0) * m;
        _lightDir.Normalize();

        _position = Vector3f(_absTrans._41, _absTrans._42, _absTrans._43);
    }

    void Light::PrepareForRendering()
    {
        if (_lightBuffer == nullptr) 
        {
            _lightBuffer = Engine::GetInstance()->GetRenderDevice()->CreateConstBuffer(sizeof(MainLight), ResourceUsage::USAGE_DYNAMIC, "MainLight");
        }

        MainLight params;
        params._lightDirAndRadius = Vector4f(_lightDir._x, _lightDir._y, _lightDir._z, _radius);
        params._lightColor = Vector4f(_lightColor._r, _lightColor._g, _lightColor._b, _shiness);

        _lightBuffer->WriteData(0, sizeof(MainLight), &params, true);
    }
}