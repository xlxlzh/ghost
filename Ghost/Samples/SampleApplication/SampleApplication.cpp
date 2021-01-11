#include "SampleApplication.h"
#include "Engine.h"
#include "MeshNode.h"
#include <windows.h>

void SampleApplication::onInit()
{
    Engine::getInstance()->initEngine(getAttachWindow(), RENDER_D3D11, _4x, _window->getWidth(), _window->getHeight());
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->setClearColor(Color::Blue);

    auto fileSystem = Engine::getInstance()->getFileSystem();
    _generateResourcesPath(fileSystem->getCurrentDir());

    ResourceManager::getInstance()->setResourcesPath(_resourcesPath);
    MaterialPtr matPtr = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/DefaultMaterial.xml", 0));

    MeshPtr mesh = GHOST_SMARTPOINTER_CAST(Mesh, ResourceManager::getInstance()->addResource(RESOURCE_MESH, "Meshes/dragon.obj", 0));

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);
    _mainCamera->setProjectParams(90, _window->getHeight() / (float)_window->getWidth(), 1.0f, 1000.0f);
    _mainCamera->setTransform(Vector3f(0.0, 40.0, -10.0), Vector3f(0.0, 0.0, 0.0), Vector3f(1.0, 1.0, 1.0));

    MeshNode* houseNode = new MeshNode(_scene);
    houseNode->setMesh(mesh);
    houseNode->setMaterial(matPtr);
    houseNode->setTransform(Vector3f(0.0, 40.0, 10.0), Vector3f(0.0, 0.0, 0.0), Vector3f(10, 10, 10));
    _scene->addNodeToRoot(houseNode);

    Light* mainLight = new Light(_scene);
    mainLight->setLightType(LIGHT_DIRECTIONAL);
    mainLight->setLightColor(Color(1.0, 1.0, 1.0));
    _scene->addNodeToRoot(mainLight);

    _scene->addNodeToRoot(_mainCamera);
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
}

void SampleApplication::_generateResourcesPath(const std::string& workdir)
{
    _resourcesPath.clear();
    _resourcesPath = AddTrailingSlash(workdir);
    _resourcesPath = _resourcesPath + "../../../Resources/";
}