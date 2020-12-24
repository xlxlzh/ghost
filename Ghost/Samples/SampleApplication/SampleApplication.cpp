#include "SampleApplication.h"
#include "Engine.h"
#include <windows.h>

void SampleApplication::onInit()
{
    Engine::getInstance()->initEngine(getAttachWindow(), RENDER_D3D11, _4x, _window->getWidth(), _window->getHeight());
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->setClearColor(Color::Blue);

    auto fileSystem = Engine::getInstance()->getFileSystem();
    _generateResourcesPath(fileSystem->getCurrentDir());

    ResourceManager::getInstance()->setResourcesPath(_resourcesPath);
    ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/DefaultMaterial.xml", 0);

    ResourceManager::getInstance()->addResource(RESOURCE_MESH, "Meshes/house.obj", 0);

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);

    _scene->addNodeToRoot(_mainCamera);

    VertexBufferPtr buffer = Engine::getInstance()->getRenderDevice()->createVertexBuffer(4, 1, BufferUsage::USAGE_DYNAMIC);
    int test = 999999;
    buffer->writeData(0, 4, &test, true);
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
    _scene->updateSceneGraph(_mainCamera);
    _scene->render(_mainCamera);
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->clearRenderTarget(TargetClear::CLEAR_ALL, Color::Blue);
    ri->endScene();
}

void SampleApplication::_generateResourcesPath(const std::string& workdir)
{
    _resourcesPath.clear();
    _resourcesPath = AddTrailingSlash(workdir);
    _resourcesPath = _resourcesPath + "../../../Resources/";
}