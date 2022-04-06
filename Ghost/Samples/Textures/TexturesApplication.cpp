#include "TexturesApplication.h"
#include "Engine.h"
#include "ModelNode.h"
#include <windows.h>

void TexturesApplication::onInit()
{
    auto ri = Engine::getInstance()->getRenderSystem();
    ri->setClearColor(Color::Gray);

    auto fileSystem = Engine::getInstance()->getFileSystem();
    _generateResourcesPath(fileSystem->getCurrentDir());

    ResourceManager::getInstance()->setResourcesPath(_resourcesPath);
    MaterialPtr matPtr = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::getInstance()->addResource(RESOURCE_MATERIAL, "Materials/DefaultMaterial.xml", 0));

    ModelPtr houseModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::getInstance()->addResource(RESOURCE_MODEL, "Meshes/miniHouse_FBX.FBX", 0));

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);
    _mainCamera->setProjectParams(90, _window->getWidth() / (float)_window->getHeight(), 1.0f, 100.0f);
    _mainCamera->setTransform(Vector3f(0.0, 40.0, 0.0), Vector3f(0.0, 0.0, 0.0), Vector3f(1.0, 1.0, 1.0));

    ModelNode* house = new ModelNode(_scene);
    house->setModel(houseModel);
    house->setMaterial(matPtr);
    house->setTransform(Vector3f(-2.0, 40.0, 10.0), Vector3f(0.0, 0.0, 0.0), Vector3f(0.01, 0.01, 0.01));
    _scene->addNodeToRoot(house);

    _mainLight = new Light(_scene);
    _mainLight->setLightType(LIGHT_DIRECTIONAL);
    _mainLight->setLightColor(Color(1.0, 1.0, 1.0));
    _mainLight->setTransform(Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, -45.0, 45.0), Vector3f(1.0, 1.0, 1.0));
    _mainLight->setLightShiness(100);
    _scene->addNodeToRoot(_mainLight);

    _scene->addNodeToRoot(_mainCamera);

    _settingPage = MAKE_SMART_POINTER(SettingPage);

    ghost::GuiManager::getInstance()->registerGuiPage(_settingPage);
}

void TexturesApplication::onExit()
{

}

void TexturesApplication::onTick(float deltaTime)
{
    char fpsTitle[128] = { 0 };
    sprintf(fpsTitle, "TexturesApplication--%0.2f", 1000.0 / deltaTime);
    getWindow()->setWindowTitle(fpsTitle);
}

void TexturesApplication::onKeydown()
{
    Engine::getInstance()->getRenderSystem()->setFillMode(FillMode::FILL_WIREFRAME);
}

void TexturesApplication::onUpdate()
{
    _mainLight->setLightColor(_settingPage->getLightColor());
    _scene->updateSceneGraph(_mainCamera);
    _scene->render(_mainCamera);
}

void TexturesApplication::_generateResourcesPath(const std::string& workdir)
{
    _resourcesPath.clear();
    _resourcesPath = AddTrailingSlash(workdir);
    _resourcesPath = _resourcesPath + "../../../Resources/";
}