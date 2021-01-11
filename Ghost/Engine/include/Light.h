#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "SceneNode.h"
#include "Color.h"
#include "ConstBuffer.h"

namespace ghost
{
    enum LightType
    {
        LIGHT_DIRECTIONAL,
        LIGHT_POINT,
        LIGHT_SPOT
    };

    class GHOST_API Light : public SceneNode
    {
        friend class SceneManager;
    public:
        Light(SceneManager* owner);
        ~Light();

        LightType getLightType() const { return _lightType; }
        void setLightType(LightType lightType) { _lightType = lightType; }

        void setSpotOuter(float outer) { _spotOuter = outer; }
        float getSpotOuter() const { return _spotOuter; }
        void setSpotInner(float inner) { _spotInner = inner; }
        float getSpotInner() const { return _spotInner; }

        Color getLightColor() const { return _lightColor; }
        void setLightColor(const Color& cl) { _lightColor = cl; }

        void setRadius(float radius) { _radius = radius; }
        float getRadius() const { return _radius; }

        void prepareForRendering();

        GET_SCENENODE_TYPE(LIGHT)

    protected:
        virtual void onPostUpdate() override;

    private:
        LightType _lightType;
        Color _lightColor;
        
        Vector3f _lightDir;
        Vector3f _position;

        float _spotOuter;
        float _spotInner;
        float _falloff;
        float _radius;

        //Rendering
        ConstBufferPtr _lightBuffer = nullptr;
    };
}

#endif
