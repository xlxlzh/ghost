#include "Renderer.h"

namespace ghost
{
    void RenderQueue::resetRenderQueue()
    {
        _opaueQueue.clear();
        _shadowQueue.clear();

        _mainCamera = nullptr;
        _directionLight = nullptr;
    }
}