#include "TexturesApplication.h"
#include "Engine.h"
#include "ModelNode.h"
#include <windows.h>

void TexturesApplication::InitSample()
{
    MaterialPtr matPart1 = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::GetInstance()->AddResource(RESOURCE_MATERIAL, "Materials/HouseMaterial_1.xml", 0));
    MaterialPtr matPart2 = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::GetInstance()->AddResource(RESOURCE_MATERIAL, "Materials/HouseMaterial_2.xml", 0));
    MaterialPtr matPart3 = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::GetInstance()->AddResource(RESOURCE_MATERIAL, "Materials/HouseMaterial_3.xml", 0));
    MaterialPtr matPart4 = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::GetInstance()->AddResource(RESOURCE_MATERIAL, "Materials/HouseMaterial_4.xml", 0));

    ModelPtr houseModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::GetInstance()->AddResource(RESOURCE_MODEL, "Meshes/miniHouse_FBX.FBX", 0));

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);
    _mainCamera->SetProjectParams(90, _window->GetWidth() / (float)_window->GetHeight(), 1.0f, 100.0f);
    _mainCamera->SetTransform(Vector3f(0.0, 37.0, -5.0), Vector3f(0.0, 0.0, 0.0), Vector3f(1.0, 1.0, 1.0));

    ModelNode* house = new ModelNode(_scene);
    house->SetModel(houseModel);
    houseModel->SetMaterial(0, matPart4);
    houseModel->SetMaterial(1, matPart3);
    houseModel->SetMaterial(2, matPart2);
    houseModel->SetMaterial(3, matPart1);
    houseModel->SetMaterial(4, matPart1);
    houseModel->SetMaterial(5, matPart3);
    houseModel->SetMaterial(6, matPart1); 
    house->SetTransform(Vector3f(0.0, 35.0, 10.0), Vector3f(0.0, 90, 0.0), Vector3f(0.015, 0.015, 0.015));
    _scene->AddNodeToRoot(house);

    _mainLight = new Light(_scene);
    _mainLight->SetLightType(LIGHT_DIRECTIONAL);
    _mainLight->SetLightColor(Color(1.0, 1.0, 1.0));
    _mainLight->SetTransform(Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, -45.0, 45.0), Vector3f(1.0, 1.0, 1.0));
    _mainLight->SetLightShiness(100);
    _scene->AddNodeToRoot(_mainLight);

    _scene->AddNodeToRoot(_mainCamera);

    _settingPage = GHOST_MAKE_SMART_POINTER(SettingPage);

    ghost::GuiManager::GetInstance()->RegisterGuiPage(_settingPage);
}

void TexturesApplication::OnKeydown(GhostKey key)
{
    Engine::GetInstance()->GetRenderSystem()->SetFillMode(FillMode::FILL_WIREFRAME);
}

void TexturesApplication::OnUpdate()
{
    _mainLight->SetLightColor(_settingPage->GetLightColor());
    _scene->UpdateSceneGraph(_mainCamera);
    _scene->Render(_mainCamera);
}