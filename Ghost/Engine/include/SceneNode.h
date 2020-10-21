#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "Ghost.h"
#include "BoundingBox.h"
#include <vector>

namespace ghost
{
    enum SceneNodeType
    {
        SCENENODE_UNDEFINED,
        SCENENODE_CAMERA,
        SCENENODE_LIGHT
    };

    class Octree;

    class GHOST_API SceneNode
    {
    public:
        SceneNode() = default;
        virtual ~SceneNode() = default;

        BoundingBox& getBoundingBox() { return _boundingBox; }
        SceneNode* getParent() const { return _parent; }

        void setTree(Octree* tree) { _tree = tree; }
        Octree* getTree() const { return _tree; }

    protected:
        std::string _name;
        BoundingBox _boundingBox;

        SceneNode* _parent;
        Octree* _tree;
    };
}

#endif
