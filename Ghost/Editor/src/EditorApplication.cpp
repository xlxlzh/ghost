#include "EditorApplication.h"

#include "Engine.h"
#include "GUIManager.h"

void EditorApplication::onInit()
{
    auto ri = ghost::Engine::getInstance()->getRenderSystem();
    ri->setClearColor(ghost::Color::Gray);
    
    _contentBrowserPage = GHOST_MAKE_SMART_POINTER(ContentBrowserPage);
    ghost::GuiManager::getInstance()->registerGuiPage(_contentBrowserPage);
}

void EditorApplication::onExit()
{
    
}

void EditorApplication::onUpdate()
{
    const auto& renderSystem = ghost::Engine::getInstance()->getRenderSystem();
    renderSystem->beginScene();
    ghost::GuiManager::getInstance()->renderAllPages();
    renderSystem->endScene();
}

void EditorApplication::onTick(float deltaTime)
{

}

