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

        void SetVisible(bool v) { _visible; }
        void SetAcceptShadow(bool s) { _acceptShdow = s; }
        void SetProjectShadow(bool s) { _projShadow = s; }

        bool GetVisible() const { return _visible; }
        bool GetAcceptShadow() const { return _acceptShdow; }
        bool GetProjectShadow() const { return _projShadow; }

    protected:
        bool _visible{ true };
        bool _acceptShdow{ true };
        bool _projShadow{ true };
    };
}

#endif
