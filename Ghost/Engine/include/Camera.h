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

        void setProjectParams(float fov, float aspect, float n, float f);
        void setViewport(const ViewportPtr& vPtr);
        
        const Matrix4x4f& getViewMatrix() const { return _matView; }
        const Vector3f& getAbsPos() const { return _absPos; }
        const Matrix4x4f& getProjectMatrix() const { return _matProj; }

        const Frustum& getFrustum() const { return _frustum; }

        void prepareForRendering();

        GET_SCENENODE_TYPE(CAMERA)

    protected:
        virtual void onPostUpdate() override;

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
