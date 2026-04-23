#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Ghost.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"

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

        void AddNodeToRoot(SceneNode* node);
        void AddNode(SceneNode* node, SceneNode* parent);

        void UpdateSceneGraph(Camera* camera);

        void Render(Camera* camera);

        SceneNode* GetRootNode() const { return _sceneNodes[0]; }

        BoundingBox& GetBoundingBox() { return _boundingBox; }

        void UpdateNode(SceneNode* node);

        void SetAmbientColor(const Color& cl) { _ambientColor = cl; }
        void PrepareRendering();

        Light* GetMainLigt() const;

        const Matrix4x4f& GetShadowViewMat() const { return _lightViewMat; }
        const Matrix4x4f& GetShadowProjMat() const { return _lightProjMat; }

    private:
        void InitTree(const BoundingBox& box, int depth);

        void AddNodeToTree(SceneNode* node, Octree* octree, int depth = 0);
        bool DeleteNodeFromTree(SceneNode* node);

        void UpdateRenderQueue();
        void RenderShadowmap(Camera* camera, Light* light);
        void GetShadowmapRenderObjects(Light* light, std::vector<SceneNode*>& nodes);

    private:
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
