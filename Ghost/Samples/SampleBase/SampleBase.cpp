#include "SampleBase.h"
#include "Engine.h"


void SampleBase::OnInit()
{
    auto ri = Engine::GetInstance()->GetRenderSystem();
    ri->SetClearColor(Color::Gray);

    auto fileSystem = Engine::GetInstance()->GetFileSystem();
    GenerateResourcesPath(fileSystem->GetCurrentDir());
    ResourceManager::GetInstance()->SetResourcesPath(_resourcesPath);

    InitSample();
}

void SampleBase::OnExit()
{

}

void SampleBase::OnTick(float deltaTime)
{
    char fpsTitle[128] = { 0 };
    sprintf(fpsTitle, "SampleApplication FPS:%0.2f", 1000.0 / deltaTime);
    GetWindow()->SetWindowTitle(fpsTitle);
}

void SampleBase::GenerateResourcesPath(const std::string& workdir)
{
    _resourcesPath.clear();
    _resourcesPath = AddTrailingSlash(workdir);
    _resourcesPath = _resourcesPath + "../../../Resources/";
}