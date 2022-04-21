#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "Ghost.h"
#include "SceneNode.h"

namespace ghost
{
    class GHOST_API Renderable : public SceneNode
    {
    public:
        Renderable(SceneManager* owner) : SceneNode(owner) { }



    protected:
        bool _visible{ true };
        bool _acceptShdow{ true };
        bool _projShadow{ true };
    };
}

#endif
