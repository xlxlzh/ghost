#include "SceneManager.h"
#include "Octree.h"
#include "Engine.h"
#include "Renderable.h"
#include "ModelNode.h"
#include "Model.h"
#include "Material.h"
#include "Resource.h"
#include "ShaderConstBufferStruct.h"
#include "ForwardRenderer.h"
#include "PostprocessingManager.h"
#include "LogManager.h"

#include "GUIManager.h"

#include <algorithm>

namespace ghost
{
    SceneManager::SceneManager()
    {
        Initialize(BoundingBox(-10000, -10000, -10000, 10000, 10000, 10000), 8);
    }

    SceneManager::SceneManager(const BoundingBox& box, int depth)
    {
        Initialize(box, depth);
    }

    void SceneManager::Initialize(const BoundingBox& box, int depth)
    {
        SceneNode* root = new GroupNode(this);
        _sceneNodes.push_back(root);

        _octree = nullptr;
        InitTree(box, depth);

        _ambientColor = Color(0.2f, 0.2f, 0.2f);
    }

    SceneManager::~SceneManager()
    {
        Clear();

        if (!_sceneNodes.empty())
        {
            delete _sceneNodes[0];
            _sceneNodes.clear();
        }

        _lights.clear();
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

    void SceneManager::RemoveNode(SceneNode* node)
    {
        if (!node || _sceneNodes.empty() || node == _sceneNodes[0])
            return;

        std::vector<SceneNode*> children = node->_children;
        for (SceneNode* child : children)
            RemoveNode(child);

        if (node->_parent)
        {
            std::vector<SceneNode*>& siblings = node->_parent->_children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), node), siblings.end());
            node->_parent = nullptr;
        }

        if (Octree* tree = node->GetTree())
        {
            tree->RemoveNode(node);
            node->SetTree(nullptr);
        }

        if (node->GetType() == SCENENODE_LIGHT)
        {
            Light* light = static_cast<Light*>(node);
            _lights.erase(std::remove(_lights.begin(), _lights.end(), light), _lights.end());
        }

        _sceneNodes.erase(std::remove(_sceneNodes.begin(), _sceneNodes.end(), node), _sceneNodes.end());
        delete node;
    }

    void SceneManager::Clear()
    {
        if (_sceneNodes.empty())
            return;

        SceneNode* root = _sceneNodes[0];

        std::vector<SceneNode*> children = root->_children;
        for (SceneNode* child : children)
            RemoveNode(child);

        while (_sceneNodes.size() > 1)
        {
            SceneNode* node = _sceneNodes.back();

            if (node == root)
            {
                _sceneNodes.pop_back();
                continue;
            }

            const size_t sizeBefore = _sceneNodes.size();
            RemoveNode(node);

            if (_sceneNodes.size() == sizeBefore)
                _sceneNodes.pop_back();
        }

        _lights.clear();

        if (root)
            root->SetSourceData(nullptr);
    }

    void SceneManager::SetScene(const ScenePtr& scene)
    {
        Clear();

        _scene = scene;

        if (_sceneNodes.empty())
            return;

        SceneNode* root = _sceneNodes[0];
        root->SetSourceData(nullptr);

        if (!_scene || !_scene->GetRoot())
            return;

        root->SetSourceData(_scene->GetRoot());
        root->SetName(_scene->GetRoot()->name);

        for (auto& child : _scene->GetRoot()->children)
            InstantiateNode(child.get(), root);
    }

    SceneNode* SceneManager::InstantiateNode(SceneNodeData* data, SceneNode* parent)
    {
        if (!data)
            return nullptr;

        if (SceneNode* existing = FindNodeByData(data))
            return existing;

        if (!parent)
            parent = GetRootNode();

        if (!parent)
            return nullptr;

        SceneNode* node = nullptr;
        switch (data->type)
        {
        case SceneNodeDataType::Group:  node = new GroupNode(this); break;
        case SceneNodeDataType::Camera: node = new Camera(this); break;
        case SceneNodeDataType::Light:  node = new Light(this); break;
        case SceneNodeDataType::Model:  node = new ModelNode(this); break;
        }

        if (!node)
            return nullptr;

        node->SetSourceData(data);
        node->SetName(data->name);

        if (Camera* camera = dynamic_cast<Camera*>(node))
        {
            const float aspect = data->cameraAspect > 0.0f ? data->cameraAspect : 1.0f;
            camera->SetProjectParams(data->cameraFov, aspect, data->cameraNear, data->cameraFar);
        }
        else if (Light* light = dynamic_cast<Light*>(node))
        {
            light->SetLightType(data->lightType);
            light->SetLightColor(data->color);
            light->SetLightShiness(data->shininess);
            light->SetRadius(data->radius);
            light->SetSpotInner(data->spotInner);
            light->SetSpotOuter(data->spotOuter);
            light->SetLightProjectShadow(data->projectShadow);
        }
        else if (Renderable* renderable = dynamic_cast<Renderable*>(node))
        {
            renderable->SetVisible(data->visible);
            renderable->SetAcceptShadow(data->acceptShadow);
            renderable->SetProjectShadow(data->projectShadow);
        }

        if (ModelNode* modelNode = dynamic_cast<ModelNode*>(node))
        {
            if (!data->modelPath.empty())
            {
                const std::string materialPath = data->materialPath.empty()
                    ? "Materials/DefaultMaterial.xml" : data->materialPath;

                ModelPtr model = GHOST_SMARTPOINTER_CAST(Model,
                    ResourceManager::GetInstance()->AddResource(RESOURCE_MODEL, data->modelPath, 0));
                MaterialPtr material = GHOST_SMARTPOINTER_CAST(Material,
                    ResourceManager::GetInstance()->AddResource(RESOURCE_MATERIAL, materialPath, 0));

                if (model && material && model->GetSubMeshCount() > 0)
                {
                    for (unsigned index = 0; index < model->GetSubMeshCount(); ++index)
                        model->SetMaterial(index, material);

                    modelNode->SetModel(model);
                }
                else
                {
                    GHOST_LOG_FORMAT_WARNNING(
                        "Scene: failed to instantiate model '%s' with material '%s'.",
                        data->modelPath.c_str(), materialPath.c_str());
                }
            }
        }

        node->SetTransform(data->position, data->rotation, data->scale);
        AddNode(node, parent);

        for (auto& child : data->children)
            InstantiateNode(child.get(), node);

        return node;
    }

    SceneNode* SceneManager::FindNodeByData(SceneNodeData* data) const
    {
        if (!data)
            return nullptr;

        for (SceneNode* node : _sceneNodes)
        {
            if (node && node->GetSourceData() == data)
                return node;
        }

        return nullptr;
    }

    void SceneManager::SyncNodeToData(SceneNode* node)
    {
        if (!node || !node->GetSourceData())
            return;

        SceneNodeData* data = node->GetSourceData();

        data->name = node->GetName();
        node->GetTransform(data->position, data->rotation, data->scale);

        if (Renderable* renderable = dynamic_cast<Renderable*>(node))
        {
            data->visible = renderable->GetVisible();
            data->acceptShadow = renderable->GetAcceptShadow();
            data->projectShadow = renderable->GetProjectShadow();
        }

        if (Light* light = dynamic_cast<Light*>(node))
        {
            data->lightType = light->GetLightType();
            data->color = light->GetLightColor();
            data->shininess = light->GetLightShiness();
            data->radius = light->GetRadius();
            data->spotInner = light->GetSpotInner();
            data->spotOuter = light->GetSpotOuter();
            data->projectShadow = light->GetLightProjectShadow();
        }

        if (Camera* camera = dynamic_cast<Camera*>(node))
        {
            data->cameraFov = camera->GetFov();
            data->cameraAspect = camera->GetAspect();
            data->cameraNear = camera->GetNear();
            data->cameraFar = camera->GetFar();
        }

        if (ModelNode* modelNode = dynamic_cast<ModelNode*>(node))
        {
            const ModelPtr& model = modelNode->GetMesh();
            if (model)
            {
                data->modelPath = model->GetName();

                if (model->GetSubMeshCount() > 0)
                {
                    const MaterialPtr& material = model->GetMaterial(0);
                    if (material)
                        data->materialPath = material->GetName();
                }
            }
        }
    }

    void SceneManager::SyncAllToData()
    {
        for (SceneNode* node : _sceneNodes)
        {
            if (node && !node->IsEditorOnly())
                SyncNodeToData(node);
        }
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
        if (!_octree || !node || node == GetRootNode())
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
            {
                ModelNode* modelNode = static_cast<ModelNode*>(node);
                if (modelNode->GetVisible() && modelNode->GetProjectShadow())
                    nodes.push_back(node);
            }
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
            if (sc && sc->GetType() == SCENENODE_MODEL)
            {
                ModelNode* modelNode = static_cast<ModelNode*>(sc);
                if (!modelNode->GetVisible())
                    continue;
            }

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