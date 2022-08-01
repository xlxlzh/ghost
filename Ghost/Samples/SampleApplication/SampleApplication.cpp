#include "SampleApplication.h"
#include "Engine.h"
#include "ModelNode.h"
#include <windows.h>

void SampleApplication::initSample()
{
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
    //_scene->addNodeToRoot(dragonNode);

    ModelNode* bunny = new ModelNode(_scene);
    bunny->setModel(bunnyModel);
    bunnyModel->setMaterial(0, matPtr);
    bunny->setTransform(Vector3f(2, 40.0, 10.0), Vector3f(0.0, 30.0, 0.0), Vector3f(30, 30, 30));
    //_scene->addNodeToRoot(bunny);

    ModelNode* head = new ModelNode(_scene);
    head->setModel(headModel);
    headModel->setMaterial(0, headMatPtr);
    head->setTransform(Vector3f(7, 40.0, 10.0), Vector3f(0.0, 90.0, 0.0), Vector3f(0.5, 0.5, 0.5));
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

void SampleApplication::onKeydown(GhostKey key)
{
    if (key == GhostKey::GHOST_KEY_1)
    {
        _wireFrame = !_wireFrame;
        Engine::getInstance()->getRenderSystem()->setFillMode(_wireFrame ? FillMode::FILL_WIREFRAME : FillMode::FILL_SOLID);
    }
        
    if (key == GhostKey::GHOST_KEY_2)
    {
        _enableZTest = !_enableZTest;
        Engine::getInstance()->getRenderSystem()->setDepthTestEnable(_enableZTest);
    }

    if (key == GhostKey::GHOST_KEY_3)
    {
        _lessgreate = !_lessgreate;
        Engine::getInstance()->getRenderSystem()->setDepthFunction(_lessgreate ? CompareFunction::COMPARISON_LESS_EQUAL : CompareFunction::COMPARISON_GREATER_EQUAL);
    }
       
}

void SampleApplication::onUpdate()
{
    _mainLight->setLightColor(_settingPage->getLightColor());
    _scene->updateSceneGraph(_mainCamera);
    _scene->render(_mainCamera);
}