#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "Ghost.h"
#include "BoundingBox.h"
#include <vector>
#include <string>
#include "Matrix4x4.h"
#include "RenderOperation.h"

namespace ghost
{
    enum SceneNodeType
    {
        SCENENODE_UNDEFINED,
        SCENENODE_CAMERA,
        SCENENODE_LIGHT,
        SCENENODE_MODEL
    };

    #define GET_SCENENODE_TYPE(NodeType) virtual SceneNodeType GetType() const { return SCENENODE_##NodeType; }

    class Octree;
    class SceneManager;
    class Camera;

    class GHOST_API SceneNode
    {
        friend class SceneManager;
    public:
        SceneNode(SceneManager* owner);
        virtual ~SceneNode() = default;

        BoundingBox& GetBoundingBox() { return _boundingBox; }
        SceneNode* GetParent() const { return _parent; }

        void SetTree(Octree* tree) { _tree = tree; }
        Octree* GetTree() const { return _tree; }

        Matrix4x4f& GetRelTrans() { return _relTrans; }
        Matrix4x4f& GetAbsTrans() { return _absTrans; }
        void SetTransform(const Vector3f& pos, const Vector3f& rotation, const Vector3f& scale);
        void SetTransform(const Matrix4x4f& mat);
        void SetPosition(const Vector3f& pos);
        void GetTransform(Vector3f& pos, Vector3f& rotation, Vector3f& scale);
        void SetScale(const Vector3f& scale);
        void SetRotation(const Vector3f&rotation);

        void Update();

        void MarkDirty();
        void MarkChildrenDirty();

        bool IsIn(const BoundingBox& inBox) const { return true; }

        virtual void Render(Camera* cam) { }

        //virtual void getRenderOperation(RenderOperation& op) { }

        GET_SCENENODE_TYPE(UNDEFINED)

    protected:
        virtual void OnPostUpdate() { }

    protected:
        std::string _name;
        BoundingBox _boundingBox;

        std::vector<SceneNode*> _children;
        SceneNode* _parent;
        Octree* _tree;

        //Transform in the world
        Matrix4x4f _relTrans;
        Vector3f _pos, _scale, _rotation;

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
