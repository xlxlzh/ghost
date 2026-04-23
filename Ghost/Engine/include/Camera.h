#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "SceneNode.h"
#include "Viewport.h"
#include "ConstBuffer.h"
#include "Frustum.h"

namespace ghost
{
    class GHOST_API Camera final : public SceneNode
    {
        friend class SceneManager;
    public:
        Camera(SceneManager* owner) : SceneNode(owner) { }
        virtual ~Camera() { }

        void SetProjectParams(float fov, float aspect, float n, float f);
        void SetViewport(const ViewportPtr& vPtr);
        
        const Matrix4x4f& GetViewMatrix() const { return _matView; }
        const Vector3f& GetAbsPos() const { return _absPos; }
        const Matrix4x4f& GetProjectMatrix() const { return _matProj; }

        const Frustum& GetFrustum() const { return _frustum; }

        void PrepareForRendering();

        GET_SCENENODE_TYPE(CAMERA)

    protected:
        virtual void OnPostUpdate() override;

    private:
        ViewportPtr _viewport;

        Matrix4x4f _matView;
        Matrix4x4f _matProj;
        Vector3f _absPos;

        Frustum _frustum;

        float _frustNear, _frustFar;
        float _frustLeft, _frustRight;
        float _frustTop, _frustBottom;
        float _fov, _aspect;

        //Perspective or orthographic
        bool _orthographic{ false };

        ConstBufferPtr _cameraParams = nullptr;
    };
}

#endif
