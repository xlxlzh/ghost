#include "SceneManager.h"
#include "Octree.h"
#include "Engine.h"
#include "Renderable.h"
#include "ShaderConstBufferStruct.h"
#include "ForwardRenderer.h"

#include "GUIManager.h"

namespace ghost
{
    SceneManager::SceneManager()
    {
        BoundingBox box(-10000, -10000, -10000, 10000, 10000, 10000);

        SceneNode* root = new GroupNode(this);
        _sceneNodes.push_back(root);

        _octree = nullptr;
        _initTree(box, 8);

        _ambientColor = Color(0.2f, 0.2f, 0.2f);
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
        {
            _sceneNodes.push_back(node);
            if (node->getType() == SCENENODE_LIGHT)
                _lights.push_back((Light*)node);
        }

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

    Light* SceneManager::getMainLigt() const
    {
        if (_lights.empty())
            return nullptr;

        for (unsigned i = 0; i < _lights.size(); ++i)
        {
            Light* l = _lights[i];
            if (l->getLightType() == LIGHT_DIRECTIONAL)
                return l;
        }

        return _lights[0];
    }

    void SceneManager::_getShadowmapRenderObjects(Light* light, std::vector<SceneNode *>& nodes)
    {
        for (unsigned i = 0; i < _sceneNodes.size(); ++i)
        {
            SceneNode* node = _sceneNodes[i];
            if (node && node->getType() == SCENENODE_MODEL)
                nodes.push_back(node);
        }
    }

    void SceneManager::_renderShadowmap(Camera* camera, Light* light)
    {
        if (!light)
            return;

        std::vector<SceneNode*> objects;
        _getShadowmapRenderObjects(light, objects);

        //Prepare rt and flags
        auto renderSystem = Engine::getInstance()->getRenderSystem();
        renderSystem->setRenderPass(RENDER_PASS_SHADOW);
        renderSystem->setRenderTarget(_shadowMap);

        //New implement
        const FrustumCorners& cameraCorners = camera->getFrustum().getFrustumCorners();
        Matrix4x4f lightView = light->getViewMatrix();
        FrustumCorners frustumInLight;

        for (unsigned i = 0; i < FRUSTUM_CORNER_NUM; ++i)
            frustumInLight[i] = cameraCorners[i] * lightView;

        //Calculate light frustum
        Vector3f vMin = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
        Vector3f vMax = Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (const auto& cor : frustumInLight)
        {
            vMin._x = std::min(vMin._x, cor._x);
            vMin._y = std::min(vMin._y, cor._y);
            vMin._z = std::min(vMin._z, cor._z);

            vMax._x = std::max(vMax._x, cor._x);
            vMax._y = std::max(vMax._y, cor._y);
            vMax._z = std::max(vMax._z, cor._z);
        }

        //Transform to world
        frustumInLight[0] = vMin;
        frustumInLight[1] = Vector3f(vMin._x, vMax._y, vMin._z);
        frustumInLight[2] = Vector3f(vMax._x, vMax._y, vMin._z);
        frustumInLight[3] = Vector3f(vMax._x, vMin._y, vMin._z);
        frustumInLight[4] = Vector3f(vMin._x, vMin._y, vMax._z);
        frustumInLight[5] = Vector3f(vMin._x, vMax._y, vMax._z);
        frustumInLight[6] = vMax;
        frustumInLight[7] = Vector3f(vMax._x, vMin._y, vMax._z);

        Vector3f lightPos = ((vMin + frustumInLight[2]) / 2.0f) * lightView.inverse();
        _lightViewMat = Matrix4x4f::viewMatrix(lightPos, light->getLightDir(), Vector3f(0.0, 1.0f, 0.0f));
        _lightProjMat = Matrix4x4f::orthoMatrix(vMax._x - vMin._x, vMax._y - vMin._y, vMin._z, vMax._z);

        //TODO How to apply const buffers for muti-pass
        for (const auto& obj : objects)
            obj->render(camera);
    }

    void SceneManager::_updateRenderQueue()
    {
        _renderQueues.resetRenderQueue();
        for (unsigned i = 0; i < _sceneNodes.size(); ++i)
        {
            SceneNode* node = _sceneNodes[i];
            if (node && node->getType() == SCENENODE_MODEL)
            {
                Renderable* r = (Renderable*)node;

                _renderQueues._opaueQueue.push_back(node);

                if (r->getProjectShadow())
                    _renderQueues._shadowQueue.push_back(node);
            }
                
        }
    }

    void SceneManager::prepareRendering()
    {
        if (_sceneGlobalBuffer == nullptr)
            _sceneGlobalBuffer = Engine::getInstance()->getRenderDevice()->createConstBuffer(sizeof(SceneGlobalParams), ResourceUsage::USAGE_DYNAMIC, "SceneGlobalParams");

        SceneGlobalParams params;
        params._ambientColor = Vector4f(_ambientColor._r, _ambientColor._g, _ambientColor._b, 1.0);
        _sceneGlobalBuffer->writeData(0, sizeof(SceneGlobalParams), &params, true);

        if (_shadowMap == nullptr)
            _shadowMap = Engine::getInstance()->getRenderDevice()->createSingleRenderTarget(1024, 1024, GHOST_FORMAT_R8G8B8A8);
            
    }

    void SceneManager::updateSceneGraph(Camera* camera)
    {
        (void)camera;
        getRootNode()->update();
    }

    void SceneManager::render(Camera* camera)
    {
        auto renderSystem = Engine::getInstance()->getRenderSystem();

        GHOST_GPU_EVENT_SCOPE(renderSystem, RenderScene);

        prepareRendering();
        renderSystem->setConstBuffer(SHADER_PS, _sceneGlobalBuffer);

        camera->prepareForRendering();
        renderSystem->setConstBuffer(SHADER_PS, camera->_cameraParams);

        Light* mainLight = getMainLigt();
        if (mainLight)
        {
            mainLight->prepareForRendering();
            renderSystem->setConstBuffer(SHADER_PS, mainLight->_lightBuffer);
        }

        _updateRenderQueue();
        _renderQueues._mainCamera = camera;
        _renderQueues._directionLight = mainLight;

        
        ForwardRenderer render{};
        render.render(_renderQueues);

        renderSystem->beginScene();

        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderZPrepass);
        //TODO Z-Prepass
        GHOST_GPU_EVENT_END(renderSystem);

        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderShadowmap);
        //Render Shadowmap
        renderSystem->clearRenderTarget(CLEAR_ALL, renderSystem->getClearColor());
        _renderShadowmap(camera ,mainLight);
        GHOST_GPU_EVENT_END(renderSystem);

        renderSystem->useDefaultRenderTarget();
        
        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderOpaue);
        renderSystem->setRenderPass(RENDER_PASS_FORWARD);
        renderSystem->clearRenderTarget(CLEAR_ALL, renderSystem->getClearColor());

        //Now, we don't cull scene, just render all the objects. I will do other works later.
        for (auto& sc : _sceneNodes)
        {
            sc->render(camera);
        }

        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderTranslucency);
        //TODO RenderTranslucency
        GHOST_GPU_EVENT_END(renderSystem);

        GHOST_GPU_EVENT_BEGIN(renderSystem, PostProcessing);
        //TODO Post processing
        GHOST_GPU_EVENT_END(renderSystem);

        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderImgui);
        //Test IMGUI
        GuiManager::getInstance()->renderAllPages();
        GHOST_GPU_EVENT_END(renderSystem);

        renderSystem->endScene();
    }
}