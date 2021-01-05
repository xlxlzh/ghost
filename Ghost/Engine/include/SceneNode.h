#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "Ghost.h"
#include "BoundingBox.h"
#include <vector>
#include "Matrix4x4.h"

namespace ghost
{
    enum SceneNodeType
    {
        SCENENODE_UNDEFINED,
        SCENENODE_CAMERA,
        SCENENODE_LIGHT
    };

    class Octree;
    class SceneManager;
    class Camera;

    class GHOST_API SceneNode
    {
        friend class SceneManager;
    public:
        SceneNode(SceneManager* owner);
        virtual ~SceneNode() = default;

        BoundingBox& getBoundingBox() { return _boundingBox; }
        SceneNode* getParent() const { return _parent; }

        void setTree(Octree* tree) { _tree = tree; }
        Octree* getTree() const { return _tree; }

        Matrix4x4f& getRelTrans() { return _relTrans; }
        Matrix4x4f& getAbsTrans() { return _absTrans; }
        void setTransform(const Vector3f& pos, const Vector3f& rotation, const Vector3f& scale);
        void setTransform(const Matrix4x4f& mat);
        void getTransform(Vector3f& pos, Vector3f& rotation, Vector3f& scale);

        void update();

        void markDirty();
        void markChildrenDirty();

        bool isIn(const BoundingBox& inBox) const { return true; }

        virtual SceneNodeType getType() const { return SCENENODE_UNDEFINED; }

        virtual void render(Camera* cam) { }

    protected:
        std::string _name;
        BoundingBox _boundingBox;

        std::vector<SceneNode*> _children;
        SceneNode* _parent;
        Octree* _tree;

        //Transform in the world
        Matrix4x4f _relTrans;

        //Transform by parent
        Matrix4x4f _absTrans;

        bool _dirty;

        SceneManager* _owner;
    };


    class GHOST_API GroupNode : public SceneNode
    {
    public:
        GroupNode(SceneManager* owner) : SceneNode(owner) { }

        virtual ~GroupNode() = default;
    };
}

#endif
