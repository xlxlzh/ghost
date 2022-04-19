#ifndef _FORWARD_RENDERER_H_
#define _FORWARD_RENDERER_H_

#include "Renderer.h"

namespace ghost
{
    class GHOST_API ForwardRenderer final : public Renderer
    {
    public:
        virtual void renderOpaueObjects() override;
        virtual void renderShadowMap() override;
        virtual void render(const RenderQueue& renderQueue) override;
    };
}

#endif
