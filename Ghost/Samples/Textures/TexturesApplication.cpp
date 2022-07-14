#include "TexturesApplication.h"
#include "Engine.h"
#include "ModelNode.h"
#include <windows.h>

void TexturesApplication::initSample()
{
    MaterialPtr matPart1 = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/HouseMaterial_1.xml", 0));
    MaterialPtr matPart2 = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/HouseMaterial_2.xml", 0));
    MaterialPtr matPart3 = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/HouseMaterial_3.xml", 0));
    MaterialPtr matPart4 = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/HouseMaterial_4.xml", 0));

    ModelPtr houseModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::getInstance()->addResource(RESOURCE_MODEL, "Meshes/miniHouse_FBX.FBX", 0));

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);
    _mainCamera->setProjectParams(90, _window->getWidth() / (float)_window->getHeight(), 1.0f, 100.0f);
    _mainCamera->setTransform(Vector3f(0.0, 37.0, -5.0), Vector3f(0.0, 0.0, 0.0), Vector3f(1.0, 1.0, 1.0));

    ModelNode* house = new ModelNode(_scene);
    house->setModel(houseModel);
    houseModel->setMaterial(0, matPart4);
    houseModel->setMaterial(1, matPart3);
    houseModel->setMaterial(2, matPart2);
    houseModel->setMaterial(3, matPart1);
    houseModel->setMaterial(4, matPart1);
    houseModel->setMaterial(5, matPart3);
    houseModel->setMaterial(6, matPart1); 
    house->setTransform(Vector3f(0.0, 35.0, 10.0), Vector3f(0.0, 90, 0.0), Vector3f(0.015, 0.015, 0.015));
    _scene->addNodeToRoot(house);

    _mainLight = new Light(_scene);
    _mainLight->setLightType(LIGHT_DIRECTIONAL);
    _mainLight->setLightColor(Color(1.0, 1.0, 1.0));
    _mainLight->setTransform(Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, -45.0, 45.0), Vector3f(1.0, 1.0, 1.0));
    _mainLight->setLightShiness(100);
    _scene->addNodeToRoot(_mainLight);

    _scene->addNodeToRoot(_mainCamera);

    _settingPage = GHOST_MAKE_SMART_POINTER(SettingPage);

    ghost::GuiManager::getInstance()->registerGuiPage(_settingPage);
}

void TexturesApplication::onKeydown(GhostKey key)
{
    Engine::getInstance()->getRenderSystem()->setFillMode(FillMode::FILL_WIREFRAME);
}

void TexturesApplication::onUpdate()
{
    _mainLight->setLightColor(_settingPage->getLightColor());
    _scene->updateSceneGraph(_mainCamera);
    _scene->render(_mainCamera);
}