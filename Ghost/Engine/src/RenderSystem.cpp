#include <cassert>
#include "RenderSystem.h"

namespace ghost
{
    void RenderSystem::setClearColor(Color cl /* = Color::Black */)
    {
        _clearColor = cl;
    }

    void RenderSystem::setMaterial(Material* mat)
    {
        if (_currentMaterial != mat)
        {
            _currentMaterial = mat;

            _currentMaterial->apply();
        }
    }
}