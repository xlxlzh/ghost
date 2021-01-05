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
    ResHandle matHanle = ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/DefaultMaterial.xml", 0);
    Material* matDef = (Material*)ResourceManager::getInstance()->getResourceByHandle(matHanle);

    ResHandle houseNodeHandle = ResourceManager::getInstance()->addResource(RESOURCE_MESH, "Meshes/house.obj", 0);
    Mesh* houseRes = (Mesh*)ResourceManager::getInstance()->getResourceByHandle(houseNodeHandle);

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);

    MeshNode* houseNode = new MeshNode(_scene);
    houseNode->setMesh(houseRes);
    houseNode->setMaterial(matDef);
    _scene->addNodeToRoot(houseNode);

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