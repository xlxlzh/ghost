#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Ghost.h"
#include <vector>

namespace ghost
{
    class SceneNode;
    class Camera;
    class Light;

    using ShadowMapQueue = std::vector<SceneNode*>;
    using OpaueQueue = std::vector<SceneNode*>;

    struct GHOST_API RenderQueue
    {
        RenderQueue() = default;
        ~RenderQueue() = default;

        void resetRenderQueue();
        
        ShadowMapQueue _shadowQueue;
        OpaueQueue _opaueQueue;

        Camera* _mainCamera;
        Light* _directionLight;
    };

    class GHOST_API Renderer
    {
    public:
        Renderer() = default;

        virtual void renderShadowMap() = 0;
        virtual void renderOpaueObjects() = 0;
        virtual void render(const RenderQueue& renderQueue) = 0;
    };
}

#endif