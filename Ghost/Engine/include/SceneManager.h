#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Ghost.h"
#include "SceneNode.h"
#include "Camera.h"

namespace ghost
{
    class GHOST_API SceneManager
    {
    public:
        SceneManager();
        SceneManager(const BoundingBox& box, int depth);
        ~SceneManager();

        void addNodeToRoot(SceneNode* node);
        void addNode(SceneNode* node, SceneNode* parent);

        void updateSceneGraph(Camera* camera);

        void render(Camera* camera);

        SceneNode* getRootNode() const { return _sceneNodes[0]; }

        BoundingBox& getBoundingBox() { return _boundingBox; }

        void updateNode(SceneNode* node);

    private:
        void _initTree(const BoundingBox& box, int depth);

        void _addNodeToTree(SceneNode* node, Octree* octree, int depth = 0);
        bool _deleteNodeFromTree(SceneNode* node);

    private:
        Octree* _octree{ nullptr };
        int _maxDepth{ 0 };

        BoundingBox _boundingBox;

        std::vector<SceneNode*> _sceneNodes;
    };
}

#endif
