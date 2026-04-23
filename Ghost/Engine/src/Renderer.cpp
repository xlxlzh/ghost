#include "Renderer.h"

namespace ghost
{
    void RenderQueue::ResetRenderQueue()
    {
        _opaueQueue.clear();
        _shadowQueue.clear();

        _mainCamera = nullptr;
        _directionLight = nullptr;
    }
}