#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "SceneNode.h"
#include "Viewport.h"
#include "ConstBuffer.h"

namespace ghost
{
    class GHOST_API Camera : public SceneNode
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

        void prepareForRendering();

        GET_SCENENODE_TYPE(CAMERA)

    protected:
        virtual void onPostUpdate() override;

    private:
        ViewportPtr _viewport;

        Matrix4x4f _matView;
        Matrix4x4f _matProj;
        Vector3f _absPos;

        float _nearPlane, _farPlane;
        float _fov, _aspect;

        ConstBufferPtr _cameraParams = nullptr;
    };
}

#endif
