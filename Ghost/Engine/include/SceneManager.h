#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Ghost.h"
#include "SceneNode.h"

namespace ghost
{
    class GHOST_API SceneManager
    {
    public:
        SceneManager();
        SceneManager(const BoundingBox& box, int depth);
        ~SceneManager();

        void addNode(SceneNode* node, Octree* octree, int depth);
        bool deleteNode(SceneNode* node);

        void update();

    private:
        void _initTree(const BoundingBox& box, int depth);

    private:
        Octree* _octree{ nullptr };
        int _maxDepth{ 0 };

        BoundingBox _boundingBox;
    };
}

#endif
