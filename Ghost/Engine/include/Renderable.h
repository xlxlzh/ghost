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

        void setVisible(bool v) { _visible; }
        void setAcceptShadow(bool s) { _acceptShdow = s; }
        void setProjectShadow(bool s) { _projShadow = s; }

        bool getVisible() const { return _visible; }
        bool getAcceptShadow() const { return _acceptShdow; }
        bool getProjectShadow() const { return _projShadow; }

    protected:
        bool _visible{ true };
        bool _acceptShdow{ true };
        bool _projShadow{ true };
    };
}

#endif
