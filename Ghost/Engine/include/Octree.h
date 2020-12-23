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

        void addNode(SceneNode* node);
        void removeNode(SceneNode* node);

        int numNodes() const { return _numNodes; }
        BoundingBox& getBoundingBox() { return _boundingBox; }

        bool isTwiceSize(const BoundingBox& box) const;

    private:
        void _increaseNumNodes();
        void _decreaseNumNodes();

        void _getChildIndexes(const BoundingBox& box, int& x, int& y, int& z) const;

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
