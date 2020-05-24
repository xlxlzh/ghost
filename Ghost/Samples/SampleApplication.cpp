#include "SampleApplication.h"
#include "Engine.h"

void SampleApplication::onInit()
{
    Engine::getInstance()->initEngine(getAttachWindow(), RENDER_D3D11, _4x, _window->getWidth(), _window->getHeight());
}

void SampleApplication::onExit()
{

}