#include <cassert>
#include "RenderSystem.h"

namespace ghost
{
    void RenderSystem::setClearColor(Color cl /* = Color::Black */)
    {
        _clearColor = cl;
    }

    void RenderSystem::setMaterial(const Material& mat)
    {
        _currentMaterial = mat;
    }
}