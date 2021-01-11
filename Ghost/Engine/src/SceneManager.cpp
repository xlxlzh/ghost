#include "SceneManager.h"
#include "Octree.h"
#include "Engine.h"

namespace ghost
{
    SceneManager::SceneManager()
    {
        BoundingBox box(-10000, -10000, -10000, 10000, 10000, 10000);

        SceneNode* root = new GroupNode(this);
        _sceneNodes.push_back(root);

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

    void SceneManager::addNodeToRoot(SceneNode* node)
    {
        if (!node)
            return;

        SceneNode* root = getRootNode();
        if (!root)
            return;

        addNode(node, root);
    }

    void SceneManager::addNode(SceneNode* node, SceneNode* parent)
    {
        if (!parent)
            parent = getRootNode();

        node->_parent = parent;
        parent->_children.push_back(node);

        auto nodeIt = std::find(_sceneNodes.begin(), _sceneNodes.end(), node);
        if (nodeIt == _sceneNodes.end())
            _sceneNodes.push_back(node);

        node->markDirty();

        _octree->addNode(node);
    }

    void SceneManager::_addNodeToTree(SceneNode* node, Octree* octree, int depth)
    {
        if (!_octree)
            return;

        _sceneNodes.push_back(node);

        const BoundingBox& nodeBounding = node->getBoundingBox();
        if (depth < _maxDepth && octree->isTwiceSize(nodeBounding))
        {
            int x, y, z;
            octree->_getChildIndexes(nodeBounding, x, y, z);
            if (octree->_children[x][y][z] == nullptr)
            {
                octree->_children[x][y][z] = new Octree(octree);

                const Vector3f& treeMin = octree->_boundingBox._min;
                const Vector3f& treeMax = octree->_boundingBox._max;
                Vector3f treeCenter = octree->_boundingBox.getCenter();
                Vector3f minV, maxV;

                if (x == 0)
                {
                    minV._x = treeMin._x;
                    maxV._x = treeCenter._x;
                }
                else
                {
                    minV._x = treeCenter._x;
                    maxV._x = treeMax._x;
                }

                if (y == 0)
                {
                    minV._y = treeMin._y;
                    maxV._y = treeCenter._y;
                }
                else
                {
                    minV._y = treeCenter._y;
                    maxV._y = treeMax._y;
                }

                if (z == 0)
                {
                    minV._z = treeMin._z;
                    maxV._z = treeCenter._z;
                }
                else
                {
                    minV._z = treeCenter._z;
                    maxV._z = treeMax._z;
                }

                octree->_children[x][y][z]->_boundingBox.setExtents(minV, maxV);
                octree->_children[x][y][z]->_halfSize = (maxV - minV) / 2.0;
            }

            _addNodeToTree(node, octree->_children[x][y][z], ++depth);
        }
        else
        {
            octree->addNode(node);
        }
    }

    bool SceneManager::_deleteNodeFromTree(SceneNode* node)
    {
        if (!_octree)
            return false;

        Octree* tree = node->getTree();
        if (tree)
            tree->removeNode(node);

        node->setTree(nullptr);

        //Only remove from octree
        //_sceneNodes.erase(std::find(_sceneNodes.begin(), _sceneNodes.end(), node));

        return true;
    }

    void SceneManager::updateNode(SceneNode* node)
    {
        const BoundingBox& box = node->getBoundingBox();
        if (!_octree)
            return;

        if (node->getTree() == nullptr)
        {
            if (!node->isIn(_octree->_boundingBox))
                _octree->addNode(node);
            else
                _addNodeToTree(node, _octree);

            return;
        }

        if (!node->isIn(node->getTree()->_boundingBox))
        {
            _deleteNodeFromTree(node);

            if (!node->isIn(_octree->_boundingBox))
                _octree->addNode(node);
            else
                _addNodeToTree(node, _octree);
        }
    }

    Light* SceneManager::_getMainLigt() const
    {
        bool hasLight = false;
        Light* mainLight = nullptr;

        for (auto& node : _sceneNodes)
        {
            if (node->getType() == SCENENODE_LIGHT)
            {
                Light* l = (Light*)node;

                if (!hasLight)
                {
                    hasLight = true;
                    mainLight = l;
                }

                if (l->getLightType() == LIGHT_DIRECTIONAL)
                {
                    mainLight = l;
                    break;
                }
            }
        }

        return mainLight;
    }

    void SceneManager::updateSceneGraph(Camera* camera)
    {
        (void)camera;
        getRootNode()->update();
    }

    void SceneManager::render(Camera* camera)
    {
        auto renderSystem = Engine::getInstance()->getRenderSystem();

        camera->prepareForRendering();
        renderSystem->setConstBuffer(SHADER_PS, camera->_cameraParams);

        Light* mainLight = _getMainLigt();
        if (mainLight)
        {
            mainLight->prepareForRendering();
            renderSystem->setConstBuffer(SHADER_PS, mainLight->_lightBuffer);
        }

        renderSystem->useDefaultRenderTarget();
        renderSystem->clearRenderTarget();

        //Now, we don't cull scene, just render all the objects. I will do other works later.
        for (auto& sc : _sceneNodes)
        {
            sc->render(camera);
        }

        renderSystem->endScene();
    }
}