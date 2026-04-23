#include "SceneManager.h"
#include "Octree.h"
#include "Engine.h"
#include "Renderable.h"
#include "ShaderConstBufferStruct.h"
#include "ForwardRenderer.h"
#include "PostprocessingManager.h"

#include "GUIManager.h"

namespace ghost
{
    SceneManager::SceneManager()
    {
        BoundingBox box(-10000, -10000, -10000, 10000, 10000, 10000);

        SceneNode* root = new GroupNode(this);
        _sceneNodes.push_back(root);

        _octree = nullptr;
        InitTree(box, 8);

        _ambientColor = Color(0.2f, 0.2f, 0.2f);
    }

    SceneManager::SceneManager(const BoundingBox& box, int depth)
    {
        _octree = nullptr;
        InitTree(box, depth);
    }

    SceneManager::~SceneManager()
    {
        SAFE_DELETE(_octree);
    }

    void SceneManager::InitTree(const BoundingBox& box, int depth)
    {
        SAFE_DELETE(_octree);

        _octree = new Octree(nullptr);
        _maxDepth = depth;
        _boundingBox = box;

        _octree->_boundingBox = box;
        _octree->_halfSize = (box._max - box._min) / 2.0;
    }

    void SceneManager::AddNodeToRoot(SceneNode* node)
    {
        if (!node)
            return;

        SceneNode* root = GetRootNode();
        if (!root)
            return;

        AddNode(node, root);
    }

    void SceneManager::AddNode(SceneNode* node, SceneNode* parent)
    {
        if (!parent)
            parent = GetRootNode();

        node->_parent = parent;
        parent->_children.push_back(node);

        auto nodeIt = std::find(_sceneNodes.begin(), _sceneNodes.end(), node);
        if (nodeIt == _sceneNodes.end())
        {
            _sceneNodes.push_back(node);
            if (node->GetType() == SCENENODE_LIGHT)
                _lights.push_back((Light*)node);
        }

        node->MarkDirty();

        _octree->AddNode(node);
    }

    void SceneManager::AddNodeToTree(SceneNode* node, Octree* octree, int depth)
    {
        if (!_octree)
            return;

        _sceneNodes.push_back(node);

        const BoundingBox& nodeBounding = node->GetBoundingBox();
        if (depth < _maxDepth && octree->IsTwiceSize(nodeBounding))
        {
            int x, y, z;
            octree->GetChildIndexes(nodeBounding, x, y, z);
            if (octree->_children[x][y][z] == nullptr)
            {
                octree->_children[x][y][z] = new Octree(octree);

                const Vector3f& treeMin = octree->_boundingBox._min;
                const Vector3f& treeMax = octree->_boundingBox._max;
                Vector3f treeCenter = octree->_boundingBox.GetCenter();
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

                octree->_children[x][y][z]->_boundingBox.SetExtents(minV, maxV);
                octree->_children[x][y][z]->_halfSize = (maxV - minV) / 2.0;
            }

            AddNodeToTree(node, octree->_children[x][y][z], ++depth);
        }
        else
        {
            octree->AddNode(node);
        }
    }

    bool SceneManager::DeleteNodeFromTree(SceneNode* node)
    {
        if (!_octree)
            return false;

        Octree* tree = node->GetTree();
        if (tree)
            tree->RemoveNode(node);

        node->SetTree(nullptr);

        //Only remove from octree
        //_sceneNodes.erase(std::find(_sceneNodes.begin(), _sceneNodes.end(), node));

        return true;
    }

    void SceneManager::UpdateNode(SceneNode* node)
    {
        const BoundingBox& box = node->GetBoundingBox();
        if (!_octree)
            return;

        if (node->GetTree() == nullptr)
        {
            if (!node->IsIn(_octree->_boundingBox))
                _octree->AddNode(node);
            else
                AddNodeToTree(node, _octree);

            return;
        }

        if (!node->IsIn(node->GetTree()->_boundingBox))
        {
            DeleteNodeFromTree(node);

            if (!node->IsIn(_octree->_boundingBox))
                _octree->AddNode(node);
            else
                AddNodeToTree(node, _octree);
        }
    }

    Light* SceneManager::GetMainLigt() const
    {
        if (_lights.empty())
            return nullptr;

        for (unsigned i = 0; i < _lights.size(); ++i)
        {
            Light* l = _lights[i];
            if (l->GetLightType() == LIGHT_DIRECTIONAL)
                return l;
        }

        return _lights[0];
    }

    void SceneManager::GetShadowmapRenderObjects(Light* light, std::vector<SceneNode *>& nodes)
    {
        for (unsigned i = 0; i < _sceneNodes.size(); ++i)
        {
            SceneNode* node = _sceneNodes[i];
            if (node && node->GetType() == SCENENODE_MODEL)
                nodes.push_back(node);
        }
    }

    void SceneManager::RenderShadowmap(Camera* camera, Light* light)
    {
        if (!light)
            return;

        std::vector<SceneNode*> objects;
        GetShadowmapRenderObjects(light, objects);

        //Prepare rt and flags
        auto renderSystem = Engine::GetInstance()->GetRenderSystem();
        renderSystem->SetRenderPass(RENDER_PASS_SHADOW);
        renderSystem->SetRenderTarget(_shadowMap);

        //New implement
        const FrustumCorners& cameraCorners = camera->GetFrustum().GetFrustumCorners();
        Matrix4x4f lightView = light->GetViewMatrix();
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

        Vector3f lightPos = ((vMin + frustumInLight[2]) / 2.0f) * lightView.Inverse();
        _lightViewMat = Matrix4x4f::ViewMatrix(lightPos, light->GetLightDir(), Vector3f(0.0, 1.0f, 0.0f));
        _lightProjMat = Matrix4x4f::OrthoMatrix(vMax._x - vMin._x, vMax._y - vMin._y, vMin._z, vMax._z);

        //TODO How to apply const buffers for muti-pass
        for (const auto& obj : objects)
            obj->Render(camera);
    }

    void SceneManager::UpdateRenderQueue()
    {
        _renderQueues.ResetRenderQueue();
        for (unsigned i = 0; i < _sceneNodes.size(); ++i)
        {
            SceneNode* node = _sceneNodes[i];
            if (node && node->GetType() == SCENENODE_MODEL)
            {
                Renderable* r = (Renderable*)node;

                _renderQueues._opaueQueue.push_back(node);

                if (r->GetProjectShadow())
                    _renderQueues._shadowQueue.push_back(node);
            }
                
        }
    }

    void SceneManager::PrepareRendering()
    {
        if (_sceneGlobalBuffer == nullptr)
            _sceneGlobalBuffer = Engine::GetInstance()->GetRenderDevice()->CreateConstBuffer(sizeof(SceneGlobalParams), ResourceUsage::USAGE_DYNAMIC, "SceneGlobalParams");

        SceneGlobalParams params;
        params._ambientColor = Vector4f(_ambientColor._r, _ambientColor._g, _ambientColor._b, 1.0);
        _sceneGlobalBuffer->WriteData(0, sizeof(SceneGlobalParams), &params, true);

        if (_shadowMap == nullptr)
            _shadowMap = Engine::GetInstance()->GetRenderDevice()->CreateSingleRenderTarget(1024, 1024, GHOST_FORMAT_R8G8B8A8);
            
    }

    void SceneManager::UpdateSceneGraph(Camera* camera)
    {
        (void)camera;
        GetRootNode()->Update();
    }

    void SceneManager::Render(Camera* camera)
    {
        auto renderSystem = Engine::GetInstance()->GetRenderSystem();

        GHOST_GPU_EVENT_SCOPE(renderSystem, RenderScene);

        PrepareRendering();
        renderSystem->SetConstBuffer(SHADER_PS, _sceneGlobalBuffer);

        camera->PrepareForRendering();
        renderSystem->SetConstBuffer(SHADER_PS, camera->_cameraParams);

        Light* mainLight = GetMainLigt();
        if (mainLight)
        {
            mainLight->PrepareForRendering();
            renderSystem->SetConstBuffer(SHADER_PS, mainLight->_lightBuffer);
        }

        UpdateRenderQueue();
        _renderQueues._mainCamera = camera;
        _renderQueues._directionLight = mainLight;

        
        ForwardRenderer render{};
        render.Render(_renderQueues);

        renderSystem->BeginScene();

        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderZPrepass);
        //TODO Z-Prepass
        GHOST_GPU_EVENT_END(renderSystem);

        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderShadowmap);
        //Render Shadowmap
        renderSystem->ClearRenderTarget(CLEAR_ALL, renderSystem->GetClearColor());
        RenderShadowmap(camera ,mainLight);
        GHOST_GPU_EVENT_END(renderSystem);

        renderSystem->UseDefaultRenderTarget();
        
        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderOpaue);
        renderSystem->SetRenderPass(RENDER_PASS_FORWARD);
        renderSystem->ClearRenderTarget(CLEAR_ALL, renderSystem->GetClearColor());

        //Now, we don't cull scene, just render all the objects. I will do other works later.
        for (auto& sc : _sceneNodes)
        {
            sc->Render(camera);
        }

        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderTranslucency);
        //TODO RenderTranslucency
        GHOST_GPU_EVENT_END(renderSystem);

        GHOST_GPU_EVENT_BEGIN(renderSystem, PostProcessing);
        PostprocessingManager::GetInstance()->RenderAlleffects();
        GHOST_GPU_EVENT_END(renderSystem);

        GHOST_GPU_EVENT_BEGIN(renderSystem, RenderImgui);
        //Test IMGUI
        GuiManager::GetInstance()->RenderAllPages();
        GHOST_GPU_EVENT_END(renderSystem);

        renderSystem->EndScene();
    }
}