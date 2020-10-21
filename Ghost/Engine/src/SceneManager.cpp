#include "SceneManager.h"
#include "Octree.h"

namespace ghost
{
    SceneManager::SceneManager()
    {
        BoundingBox box(-10000, -10000, -10000, 10000, 10000, 10000);

        _octree = nullptr;
        _initTree(box, 8);
    }

    SceneManager::SceneManager(const BoundingBox& box, int depth)
    {
        _octree = nullptr;
        _initTree(box, depth);
    }

    SceneManager::~SceneManager()
    {
        SAFE_DELETE(_octree);
    }

    void SceneManager::_initTree(const BoundingBox& box, int depth)
    {
        SAFE_DELETE(_octree);

        _octree = new Octree(nullptr);
        _maxDepth = depth;
        _boundingBox = box;

        _octree->_boundingBox = box;
        _octree->_halfSize = (box._max - box._min) / 2.0;
    }

    void SceneManager::addNode(SceneNode* node, Octree* octree, int depth)
    {
        if (!_octree)
            return;


    }

    bool SceneManager::deleteNode(SceneNode* node)
    {
        return true;
    }

    void SceneManager::update()
    {

    }
}