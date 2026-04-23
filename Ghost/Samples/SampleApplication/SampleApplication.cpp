#include "SampleApplication.h"
#include "Engine.h"
#include "ModelNode.h"
#include <windows.h>

void SampleApplication::InitSample()
{
    MaterialPtr matPtr = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::GetInstance()->AddResource(RESOURCE_MATERIAL, "Materials/DefaultMaterial.xml", 0));
    MaterialPtr headMatPtr = GHOST_SMARTPOINTER_CAST(Material, ResourceManager::GetInstance()->AddResource(RESOURCE_MATERIAL, "Materials/HeadMaterial.xml", 0));

    ModelPtr dragonModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::GetInstance()->AddResource(RESOURCE_MODEL, "Meshes/dragon.obj", 0));
    ModelPtr bunnyModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::GetInstance()->AddResource(RESOURCE_MODEL, "Meshes/bunny.obj", 0));
    ModelPtr lizardModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::GetInstance()->AddResource(RESOURCE_MODEL, "Meshes/LizardMage_Lowpoly.obj", 0));
    ModelPtr cubeModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::GetInstance()->AddResource(RESOURCE_MODEL, "Meshes/cube.obj", 0));
    ModelPtr headModel = GHOST_SMARTPOINTER_CAST(Model, ResourceManager::GetInstance()->AddResource(RESOURCE_MODEL, "Meshes/head.obj", 0));

    _scene = new SceneManager();
    _mainCamera = new Camera(_scene);
    _mainCamera->SetProjectParams(90, _window->GetWidth() / (float)_window->GetHeight(), 1.0f, 15.0f);
    _mainCamera->SetTransform(Vector3f(0.0, 40.0, 0.0), Vector3f(0.0, 0.0, 0.0), Vector3f(1.0, 1.0, 1.0));

    ModelNode* dragonNode = new ModelNode(_scene);
    dragonNode->SetModel(dragonModel);
    dragonModel->SetMaterial(0, matPtr);
    dragonNode->SetTransform(Vector3f(-8.0, 40.0, 10.0), Vector3f(0.0, 30.0, 0.0), Vector3f(4, 4, 4));
    //_scene->addNodeToRoot(dragonNode);

    ModelNode* bunny = new ModelNode(_scene);
    bunny->SetModel(bunnyModel);
    bunnyModel->SetMaterial(0, matPtr);
    bunny->SetTransform(Vector3f(2, 40.0, 10.0), Vector3f(0.0, 30.0, 0.0), Vector3f(30, 30, 30));
    //_scene->addNodeToRoot(bunny);

    ModelNode* head = new ModelNode(_scene);
    head->SetModel(headModel);
    headModel->SetMaterial(0, headMatPtr);
    head->SetTransform(Vector3f(7, 40.0, 10.0), Vector3f(0.0, 90.0, 0.0), Vector3f(0.5, 0.5, 0.5));
    _scene->AddNodeToRoot(head);

    ModelNode* cube = new ModelNode(_scene);
    cube->SetModel(cubeModel);
    cubeModel->SetMaterial(0, matPtr);
    cube->SetTransform(Vector3f(-150, 30.0, 0.0), Vector3f(0.0, 0.0, 0.0), Vector3f(300, 1, 300));
    _scene->AddNodeToRoot(cube);

    _mainLight = new Light(_scene);
    _mainLight->SetLightType(LIGHT_DIRECTIONAL);
    _mainLight->SetLightColor(Color(1.0, 1.0, 1.0));
    _mainLight->SetTransform(Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, -45.0, 45.0), Vector3f(1.0, 1.0, 1.0));
    _mainLight->SetLightShiness(100);
    _scene->AddNodeToRoot(_mainLight);

    _scene->AddNodeToRoot(_mainCamera);

    _settingPage = GHOST_MAKE_SMART_POINTER(SettingPage);

    GetWindow()->SetWindowIcon("Textures/logo.jpg");

    ghost::GuiManager::GetInstance()->RegisterGuiPage(_settingPage);
}

void SampleApplication::OnKeydown(GhostKey key)
{
    if (key == GhostKey::GHOST_KEY_1)
        Engine::GetInstance()->GetRenderSystem()->SetFillMode(FillMode::FILL_WIREFRAME);
    if (key == GhostKey::GHOST_KEY_2)
        Engine::GetInstance()->GetRenderSystem()->SetDepthTestEnable(false);
}

void SampleApplication::OnUpdate()
{
    _mainLight->SetLightColor(_settingPage->GetLightColor());
    _scene->UpdateSceneGraph(_mainCamera);
    _scene->Render(_mainCamera);
}