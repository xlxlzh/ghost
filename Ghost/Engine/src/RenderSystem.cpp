#include <cassert>
#include "RenderSystem.h"

namespace ghost
{
    RenderSystem::RenderSystem() : _cullingMode(CULL_CW), _fillMode(FILL_SOLID)
    {

    }

    RenderSystem::~RenderSystem()
    {

    }

    void RenderSystem::setClearColor(Color cl /* = Color::Black */)
    {
        _clearColor = cl;
    }

    void RenderSystem::setMaterial(const MaterialPtr& mat)
    {
        if (_currentMaterial != mat)
        {
            _currentMaterial = mat;

            _currentMaterial->applyToRenderPass(_currentRenderPass);
        }
    }
}