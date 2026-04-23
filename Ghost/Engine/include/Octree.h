#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "SceneNode.h"

namespace ghost
{
    class GHOST_API Octree
    {
        friend class SceneManager;
    public:
        Octree(Octree* parent);
        ~Octree();

        void AddNode(SceneNode* node);
        void RemoveNode(SceneNode* node);

        int NumNodes() const { return _numNodes; }
        BoundingBox& GetBoundingBox() { return _boundingBox; }

        bool IsTwiceSize(const BoundingBox& box) const;

    private:
        void IncreaseNumNodes();
        void DecreaseNumNodes();

        void GetChildIndexes(const BoundingBox& box, int& x, int& y, int& z) const;

    private:
        Octree* _children[2][2][2]{ nullptr };
        Octree* _parent{ nullptr };

        BoundingBox _boundingBox;
        Vector3<float> _halfSize;

        std::vector<SceneNode*> _nodes;

        int _numNodes{ 0 };
    };
}

#endif
