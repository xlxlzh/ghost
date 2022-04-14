#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Ghost.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"

#include "RenderTarget.h"

namespace ghost
{
    class GHOST_API SceneManager
    {
        using RenderQueues = std::vector<SceneNode*>;
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

        void setAmbientColor(const Color& cl) { _ambientColor = cl; }
        void prepareRendering();

        Light* getMainLigt() const;

        const Matrix4x4f& getShadowViewMat() const { return _lightViewMat; }
        const Matrix4x4f& getShadowProjMat() const { return _lightProjMat; }

    private:
        void _initTree(const BoundingBox& box, int depth);

        void _addNodeToTree(SceneNode* node, Octree* octree, int depth = 0);
        bool _deleteNodeFromTree(SceneNode* node);

        void _getRenderQueue(RenderQueues& nodes);
        void _renderShadowmap(Camera* camera, Light* light);
        void _getShadowmapRenderObjects(Light* light, std::vector<SceneNode*>& nodes);

    private:
        Octree* _octree{ nullptr };
        int _maxDepth{ 0 };

        BoundingBox _boundingBox;

        std::vector<SceneNode*> _sceneNodes;
        std::vector<Light*> _lights;

        Color _ambientColor;

        ConstBufferPtr _sceneGlobalBuffer;

        RenderTargetPtr _shadowMap;

        RenderQueues _renderQueues;

        //Temp code
        Matrix4x4f _lightViewMat;
        Matrix4x4f _lightProjMat;
    };
}

#endif
