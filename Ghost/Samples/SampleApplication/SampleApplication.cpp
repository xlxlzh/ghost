#include "SampleApplication.h"
#include "Engine.h"
#include "MeshNode.h"
#include <windows.h>

void SampleApplication::onInit()
{
    Engine::getInstance()->initEngine(getAttachWindow(), RENDER_D3D11, _4x, _window->getWidth(), _window->getHeight());
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->setClearColor(Color::Gray);

    auto fileSystem = Engine::getInstance()->getFileSystem();
    _generateResourcesPath(fileSystem->getCurrentDir());

    ResourceManager::getInstance()->setResourcesPath(_resourcesPath);
    MaterialPtr matPtr = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/DefaultMaterial.xml", 0));

    MeshPtr dragonMesh = GHOST_SMARTPOINTER_CAST(Mesh, ResourceManager::getInstance()->addResource(RESOURCE_MESH, "Meshes/dragon.obj", 0));
    MeshPtr bunnyMesh = GHOST_SMARTPOINTER_CAST(Mesh, ResourceManager::getInstance()->addResource(RESOURCE_MESH, "Meshes/bunny.obj", 0));
    MeshPtr buddhaMesh = GHOST_SMARTPOINTER_CAST(Mesh, ResourceManager::getInstance()->addResource(RESOURCE_MESH, "Meshes/Buddha.obj", 0));
    MeshPtr cubeMesh = GHOST_SMARTPOINTER_CAST(Mesh, ResourceManager::getInstance()->addResource(RESOURCE_MESH, "Meshes/cube.obj", 0));

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);
    _mainCamera->setProjectParams(90, _window->getWidth() / (float)_window->getHeight(), 1.0f, 1000.0f);
    _mainCamera->setTransform(Vector3f(0.0, 40.0, 0), Vector3f(0.0, 0.0, 0.0), Vector3f(1.0, 1.0, 1.0));

    MeshNode* dragonNode = new MeshNode(_scene);
    dragonNode->setMesh(dragonMesh);
    dragonNode->setMaterial(matPtr);
    dragonNode->setTransform(Vector3f(-8.0, 40.0, 10.0), Vector3f(0.0, 30.0, 0.0), Vector3f(4, 4, 4));
    _scene->addNodeToRoot(dragonNode);

    MeshNode* bunny = new MeshNode(_scene);
    bunny->setMesh(bunnyMesh);
    bunny->setMaterial(matPtr);
    bunny->setTransform(Vector3f(2,40.0, 10.0), Vector3f(0.0, 30.0, 0.0), Vector3f(30, 30, 30));
    _scene->addNodeToRoot(bunny);

    MeshNode* buddha = new MeshNode(_scene);
    buddha->setMesh(buddhaMesh);
    buddha->setMaterial(matPtr);
    buddha->setTransform(Vector3f(7, 40.0, 10.0), Vector3f(0.0, 180.0, 0.0), Vector3f(5, 5, 5));
    _scene->addNodeToRoot(buddha);

    MeshNode* cube = new MeshNode(_scene);
    cube->setMesh(cubeMesh);
    cube->setMaterial(matPtr);
    cube->setTransform(Vector3f(-150, 30.0, 0.0), Vector3f(0.0, 0.0, 0.0), Vector3f(300, 1, 300));
    _scene->addNodeToRoot(cube);

    Light* mainLight = new Light(_scene);
    mainLight->setLightType(LIGHT_DIRECTIONAL);
    mainLight->setLightColor(Color(1.0, 1.0, 1.0));
    mainLight->setTransform(Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, -45.0, 45.0), Vector3f(1.0, 1.0, 1.0));
    mainLight->setLightShiness(100);
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

void SampleApplication::onKeydown()
{
    Engine::getInstance()->getRenderSystem()->setFillMode(FillMode::FILL_WIREFRAME);
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