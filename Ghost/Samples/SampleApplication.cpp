#include "SampleApplication.h"
#include "Engine.h"

void SampleApplication::onInit()
{
    Engine::getInstance()->initEngine(getAttachWindow(), RENDER_D3D11, _4x, _window->getWidth(), _window->getHeight());
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->setClearColor(Color::Blue);
}

void SampleApplication::onExit()
{

}

void SampleApplication::onUpdate()
{
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->clearRenderTarget();
    ri->endScene();
}