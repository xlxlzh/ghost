#include "SampleBase.h"
#include "Engine.h"


void SampleBase::onInit()
{
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->setClearColor(Color::Gray);

    auto fileSystem = Engine::getInstance()->getFileSystem();
    _generateResourcesPath(fileSystem->getCurrentDir());
    ResourceManager::getInstance()->setResourcesPath(_resourcesPath);

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);

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

void SampleBase::onKeydown(GhostKey key)
{
    if (key == GhostKey::GHOST_KEY_RIGHTARROW)
    {
        auto currentPos = _mainCamera->getAbsPos();
        currentPos._x += 1.0f;
        _mainCamera->setPosition(currentPos);
    }
    else if (key == GhostKey::GHOST_KEY_LEFTARROW)
    {
        auto currentPos = _mainCamera->getAbsPos();
        currentPos._x -= 1.0f;
        _mainCamera->setPosition(currentPos);
    }
    else if (key == GhostKey::GHOST_KEY_UPARROW)
    {
        auto currentPos = _mainCamera->getAbsPos();
        currentPos._z += 1.0f;
        _mainCamera->setPosition(currentPos);
    }
    else if (key == GhostKey::GHOST_KEY_DOWNARROW)
    {
        auto currentPos = _mainCamera->getAbsPos();
        currentPos._z -= 1.0f;
        _mainCamera->setPosition(currentPos);
    }

}

void SampleBase::_generateResourcesPath(const std::string& workdir)
{
    _resourcesPath.clear();
    _resourcesPath = AddTrailingSlash(workdir);
    _resourcesPath = _resourcesPath + "../../../Resources/";
}