#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "SceneNode.h"

namespace ghost
{
    class GHOST_API Camera : public SceneNode
    {
    public:
        Camera(SceneManager* owner) : SceneNode(owner) { }
        virtual ~Camera() { }

        virtual SceneNodeType getType() const  override { return SCENENODE_CAMERA; }

    private:


    };
}

#endif
