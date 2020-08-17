#include "SampleApplication.h"
#include "Engine.h"
#include <windows.h>

void SampleApplication::onInit()
{
    Engine::getInstance()->initEngine(getAttachWindow(), RENDER_D3D11, _4x, _window->getWidth(), _window->getHeight());
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->setClearColor(Color::Blue);

    auto fileSystem = Engine::getInstance()->getFileSystem();
    fileSystem->setCurrentDir("../Resources");
    //MessageBoxA(nullptr, fileSystem->getCurrentDir().c_str(), __FUNCTION__, MB_OK);
    //ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "light.material.xml", 0);
}

void SampleApplication::onExit()
{

}

void SampleApplication::onTick(float deltaTime)
{
    char fpsTitle[128] = { 0 };
    sprintf(fpsTitle, "SampleApplication--%0.2f", 1000.0 / deltaTime);
    getWindow()->setWindowTitle(fpsTitle);
}

void SampleApplication::onUpdate()
{
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->clearRenderTarget(TargetClear::CLEAR_ALL, Color::Blue);
    ri->endScene();
}