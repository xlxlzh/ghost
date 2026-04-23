#include <cassert>
#include "RenderSystem.h"

namespace ghost
{
    RenderSystem::RenderSystem(RenderDevicePtr device) : _cullingMode(CULL_CW), _fillMode(FILL_SOLID),
        _renderDevice(device)
    {

    }

    RenderSystem::~RenderSystem()
    {

    }
     
    void RenderSystem::SetClearColor(Color cl /* = Color::Black */)
    {
        _clearColor = cl;
    }

    void RenderSystem::SetMaterial(const MaterialPtr& mat)
    {
        if (_currentMaterial != mat || _renderpassChanged)
        {
            _renderpassChanged = false;
            _currentMaterial = mat;

            _currentMaterial->ApplyToRenderPass(_currentRenderPass);
        }
    }

    void RenderSystem::SetRenderPass(RenderPass pass)
    {
        _currentRenderPass = pass;
        _renderpassChanged = true;
    }
}