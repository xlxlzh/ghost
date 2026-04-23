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

    class GHOST_API Light final : public SceneNode
    {
        friend class SceneManager;
    public:
        Light(SceneManager* owner);
        ~Light();

        LightType GetLightType() const { return _lightType; }
        void SetLightType(LightType lightType) { _lightType = lightType; }

        void SetSpotOuter(float outer) { _spotOuter = outer; }
        float GetSpotOuter() const { return _spotOuter; }
        void SetSpotInner(float inner) { _spotInner = inner; }
        float GetSpotInner() const { return _spotInner; }

        Color GetLightColor() const { return _lightColor; }
        void SetLightColor(const Color& cl) { _lightColor = cl; }

        void SetRadius(float radius) { _radius = radius; }
        float GetRadius() const { return _radius; }

        void SetLightShiness(float sh) { _shiness = sh; }
        float GetLightShiness() const { return _shiness; }

        bool GetLightProjectShadow() const { return _projectShadow; }
        void SetLightProjectShadow(bool shadow) { _projectShadow = shadow; }

        // Can be used when type is directional or spot
        Vector3f GetLightDir() const;

        const Matrix4x4f& GetViewMatrix() const { return _viewMat; }

        void PrepareForRendering();

        GET_SCENENODE_TYPE(LIGHT)

    protected:
        virtual void OnPostUpdate() override;

    private:
        LightType _lightType;
        Color _lightColor;
        
        //Directional and spot use it
        Vector3f _lightDir;

        //Spot and point light use it
        Vector3f _position;

        Matrix4x4f _viewMat;

        float _spotOuter;
        float _spotInner;
        float _falloff;
        float _radius;
        float _shiness;

        bool _projectShadow{ true };

        //Rendering
        ConstBufferPtr _lightBuffer = nullptr;
    };
}

#endif
