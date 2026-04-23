#include "EditorApplication.h"

#include "Engine.h"
#include "GUIManager.h"

void EditorApplication::OnInit()
{
    auto ri = ghost::Engine::GetInstance()->GetRenderSystem();
    ri->SetClearColor(ghost::Color::Gray);
    
    _contentBrowserPage = GHOST_MAKE_SMART_POINTER(ContentBrowserPage);
    ghost::GuiManager::GetInstance()->RegisterGuiPage(_contentBrowserPage);
}

void EditorApplication::OnExit()
{
    
}

void EditorApplication::OnUpdate()
{
    const auto& renderSystem = ghost::Engine::GetInstance()->GetRenderSystem();
    renderSystem->BeginScene();
    ghost::GuiManager::GetInstance()->RenderAllPages();
    renderSystem->EndScene();
}

void EditorApplication::OnTick(float deltaTime)
{

}

