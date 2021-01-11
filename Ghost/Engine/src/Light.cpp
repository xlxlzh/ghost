#include "Light.h"
#include "Engine.h"
#include "ShaderConstBufferStruct.h"

namespace ghost
{
    Light::Light(SceneManager* owner) : SceneNode(owner)
    {
        _lightColor = Color();
    }

    Light::~Light()
    {

    }

    void Light::onPostUpdate()
    {

    }

    void Light::prepareForRendering()
    {
        if (_lightBuffer == nullptr) 
        {
            _lightBuffer = Engine::getInstance()->getRenderDevice()->createConstBuffer(sizeof(MainLight), BufferUsage::USAGE_DYNAMIC, "MainLight");
        }

        MainLight params;
        params._lightDirAndPadding = Vector4f(_lightDir._x, _lightDir._y, _lightDir._z, 1.0);
        params._lightColor = Vector4f(_lightColor._r, _lightColor._g, _lightColor._b, _lightColor._a);

        _lightBuffer->writeData(0, sizeof(MainLight), &params, true);
    }
}