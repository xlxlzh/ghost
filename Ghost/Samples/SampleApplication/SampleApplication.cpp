#include "SampleApplication.h"
#include "Engine.h"
#include "ModelNode.h"
#include <windows.h>

void SampleApplication::onInit()
{
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->setClearColor(Color::Gray);

    auto fileSystem = Engine::getInstance()->getFileSystem();
    _generateResourcesPath(fileSystem->getCurrentDir());

    ResourceManager::getInstance()->setResourcesPath(_resourcesPath);
    MaterialPtr matPtr = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/DefaultMaterial.xml", 0));
    MaterialPtr headMatPtr = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/HeadMaterial.xml", 0));

    ModelPtr dragonModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::getInstance()->addResource(RESOURCE_MODEL, "Meshes/dragon.obj", 0));
    ModelPtr bunnyModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::getInstance()->addResource(RESOURCE_MODEL, "Meshes/bunny.obj", 0));
    ModelPtr lizardModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::getInstance()->addResource(RESOURCE_MODEL, "Meshes/LizardMage_Lowpoly.obj", 0));
    ModelPtr cubeModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::getInstance()->addResource(RESOURCE_MODEL, "Meshes/cube.obj", 0));
    ModelPtr headModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::getInstance()->addResource(RESOURCE_MODEL, "Meshes/head.obj", 0));

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);
    _mainCamera->setProjectParams(90, _window->getWidth() / (float)_window->getHeight(), 1.0f, 15.0f);
    _mainCamera->setTransform(Vector3f(0.0, 40.0, 0.0), Vector3f(0.0, 0.0, 0.0), Vector3f(1.0, 1.0, 1.0));

    ModelNode* dragonNode = new ModelNode(_scene);
    dragonNode->setModel(dragonModel);
    dragonModel->setMaterial(0, matPtr);
    dragonNode->setTransform(Vector3f(-8.0, 40.0, 10.0), Vector3f(0.0, 30.0, 0.0), Vector3f(4, 4, 4));
    _scene->addNodeToRoot(dragonNode);

    ModelNode* bunny = new ModelNode(_scene);
    bunny->setModel(bunnyModel);
    bunnyModel->setMaterial(0, matPtr);
    bunny->setTransform(Vector3f(2, 40.0, 10.0), Vector3f(0.0, 30.0, 0.0), Vector3f(30, 30, 30));
    _scene->addNodeToRoot(bunny);

    ModelNode* head = new ModelNode(_scene);
    head->setModel(headModel);
    headModel->setMaterial(0, headMatPtr);
    head->setTransform(Vector3f(7, 40.0, 10.0), Vector3f(0.0, 180.0, 0.0), Vector3f(0.5, 0.5, 0.5));
    _scene->addNodeToRoot(head);

    ModelNode* cube = new ModelNode(_scene);
    cube->setModel(cubeModel);
    cubeModel->setMaterial(0, matPtr);
    cube->setTransform(Vector3f(-150, 30.0, 0.0), Vector3f(0.0, 0.0, 0.0), Vector3f(300, 1, 300));
    _scene->addNodeToRoot(cube);

    _mainLight = new Light(_scene);
    _mainLight->setLightType(LIGHT_DIRECTIONAL);
    _mainLight->setLightColor(Color(1.0, 1.0, 1.0));
    _mainLight->setTransform(Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, -45.0, 45.0), Vector3f(1.0, 1.0, 1.0));
    _mainLight->setLightShiness(100);
    _scene->addNodeToRoot(_mainLight);

    _scene->addNodeToRoot(_mainCamera);

    _settingPage = GHOST_MAKE_SMART_POINTER(SettingPage);

    getWindow()->setWindowIcon("Textures/logo.jpg");

    ghost::GuiManager::getInstance()->registerGuiPage(_settingPage);
}

void SampleApplication::onExit()
{

}

void SampleApplication::onTick(float deltaTime)
{
    char fpsTitle[128] = { 0 };
    sprintf(fpsTitle, "SampleApplication FPS:%0.2f", 1000.0 / deltaTime);
    getWindow()->setWindowTitle(fpsTitle);
}

void SampleApplication::onKeydown(GhostKey key)
{
    Engine::getInstance()->getRenderSystem()->setFillMode(FillMode::FILL_WIREFRAME);
}

void SampleApplication::onUpdate()
{
    _mainLight->setLightColor(_settingPage->getLightColor());
    _scene->updateSceneGraph(_mainCamera);
    _scene->render(_mainCamera);
}

void SampleApplication::_generateResourcesPath(const std::string& workdir)
{
    _resourcesPath.clear();
    _resourcesPath = AddTrailingSlash(workdir);
    _resourcesPath = _resourcesPath + "../../../Resources/";
}