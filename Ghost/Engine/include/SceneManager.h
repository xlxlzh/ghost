#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Ghost.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Scene.h"

#include "RenderTarget.h"
#include "Renderer.h"

namespace ghost
{
    class GHOST_API SceneManager
    {
    public:
        SceneManager();
        SceneManager(const BoundingBox& box, int depth);
        ~SceneManager();

        void SetScene(const ScenePtr& scene);
        const ScenePtr& GetScene() const { return _scene; }

        SceneNode* InstantiateNode(SceneNodeData* data, SceneNode* parent = nullptr);
        SceneNode* FindNodeByData(SceneNodeData* data) const;

        void SyncNodeToData(SceneNode* node);
        void SyncAllToData();

        void AddNodeToRoot(SceneNode* node);
        void AddNode(SceneNode* node, SceneNode* parent);

        void RemoveNode(SceneNode* node);

        void Clear();

        void UpdateSceneGraph(Camera* camera);

        void Render(Camera* camera, RenderTargetPtr renderTarget = nullptr);

        SceneNode* GetRootNode() const { return _sceneNodes[0]; }

        const std::vector<SceneNode*>& GetSceneNodes() const { return _sceneNodes; }
        const std::vector<Light*>& GetLights() const { return _lights; }

        BoundingBox& GetBoundingBox() { return _boundingBox; }

        void UpdateNode(SceneNode* node);

        void SetAmbientColor(const Color& cl) { _ambientColor = cl; }
        void PrepareRendering();

        Light* GetMainLigt() const;

        const Matrix4x4f& GetShadowViewMat() const { return _lightViewMat; }
        const Matrix4x4f& GetShadowProjMat() const { return _lightProjMat; }

    private:
        void Initialize(const BoundingBox& box, int depth);
        void InitTree(const BoundingBox& box, int depth);

        void AddNodeToTree(SceneNode* node, Octree* octree, int depth = 0);
        bool DeleteNodeFromTree(SceneNode* node);

        void UpdateRenderQueue();
        void RenderShadowmap(Camera* camera, Light* light);
        void GetShadowmapRenderObjects(Light* light, std::vector<SceneNode*>& nodes);

    private:
        ScenePtr _scene;

        Octree* _octree{ nullptr };
        int _maxDepth{ 0 };

        BoundingBox _boundingBox;

        std::vector<SceneNode*> _sceneNodes;
        std::vector<Light*> _lights;

        Color _ambientColor;

        ConstBufferPtr _sceneGlobalBuffer;

        RenderTargetPtr _shadowMap;

        RenderQueue _renderQueues;

        //Temp code
        Matrix4x4f _lightViewMat;
        Matrix4x4f _lightProjMat;
    };
}

#endif
