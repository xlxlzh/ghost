#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "SceneNode.h"
#include "Viewport.h"

namespace ghost
{
    class GHOST_API Camera : public SceneNode
    {
    public:
        Camera(SceneManager* owner) : SceneNode(owner) { }
        virtual ~Camera() { }

        virtual SceneNodeType getType() const  override { return SCENENODE_CAMERA; }

        void setProjectParams(float fov, float aspect, float n, float f);
        void setViewport(const ViewportPtr& vPtr);
        void update();
        
        const Matrix4x4f& getViewMatrix() const { return _matView; }
        const Vector3f& getAbsPos() const { return _absPos; }
        const Matrix4x4f& getProjectMatrix() const { return _matProj; }

    private:
        ViewportPtr _viewport;

        Matrix4x4f _matView;
        Matrix4x4f _matProj;
        Vector3f _absPos;

        float _nearPlane, _farPlane;
        float _fov, _aspect;
    };
}

#endif
