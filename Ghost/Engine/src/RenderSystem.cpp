#include <cassert>
#include "RenderSystem.h"

namespace ghost
{
    unsigned GetMSAASampleCount(MSAA ms)
    {
        switch (ms)
        {
        case _1x:
            return 1;
        case _2x:
            return 2;
        case _4x:
            return 4;
        case _8x:
            return 8;
        case _16x:
            return 16;
        }

        return 1;
    }

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