#ifndef _FORWARD_RENDERER_H_
#define _FORWARD_RENDERER_H_

#include "Renderer.h"

namespace ghost
{
    class GHOST_API ForwardRenderer final : public Renderer
    {
    public:
        virtual void RenderOpaueObjects() override;
        virtual void RenderShadowMap() override;
        virtual void Render(const RenderQueue& renderQueue) override;
    };
}

#endif
