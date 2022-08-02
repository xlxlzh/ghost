#include "SampleBase.h"
#include "Engine.h"


void SampleBase::onInit()
{
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->setClearColor(Color::Gray);

    auto fileSystem = Engine::getInstance()->getFileSystem();
    _generateResourcesPath(fileSystem->getCurrentDir());
    ResourceManager::getInstance()->setResourcesPath(_resourcesPath);

    initSample();
}

void SampleBase::onExit()
{

}

void SampleBase::onTick(float deltaTime)
{
    char fpsTitle[128] = { 0 };
    sprintf(fpsTitle, "SampleApplication FPS:%0.2f", 1000.0 / deltaTime);
    getWindow()->setWindowTitle(fpsTitle);
}

void SampleBase::_generateResourcesPath(const std::string& workdir)
{
    _resourcesPath.clear();
    _resourcesPath = AddTrailingSlash(workdir);
    _resourcesPath = _resourcesPath + "../../../Resources/";
}