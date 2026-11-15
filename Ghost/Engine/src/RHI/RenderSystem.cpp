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

    void RenderSystem::SetMaterial(const MaterialInterfacePtr& mat)
    {
        const uint32_t version = mat ? mat->GetVersion() : 0;

        if (_currentMaterial != mat || _currentMaterialVersion != version || _renderpassChanged)
        {
            _renderpassChanged = false;
            _currentMaterial = mat;
            _currentMaterialVersion = version;

            if (_currentMaterial)
                _currentMaterial->ApplyToRenderPass(_currentRenderPass);
        }
    }

    void RenderSystem::SetRenderPass(RenderPass pass)
    {
        _currentRenderPass = pass;
        _renderpassChanged = true;
    }
}