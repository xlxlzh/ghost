#include "EditorApplication.h"

#include <cstdio>
#include <string>

#include "Engine.h"
#include "FileSystem.h"
#include "GUIManager.h"
#include "LogManager.h"
#include "Resource.h"
#include "Scene.h"
#include "EditorScene.h"
#include "SceneSerializer.h"
#include "panels/DockspacePanel.h"

namespace
{
    std::string ResolveResourcesPath()
    {
        auto fileSystem = ghost::Engine::GetInstance()->GetFileSystem();
        const std::string currentDir = ghost::AddTrailingSlash(fileSystem->GetCurrentDir());

        const std::string candidates[] =
        {
            currentDir + "../../Resources/",
            currentDir + "../Resources/",
            currentDir + "../../../Resources/",
            currentDir + "Resources/"
        };

        for (const std::string& candidate : candidates)
        {
            if (fileSystem->DirExists(candidate + "Meshes"))
                return candidate;
        }

        return candidates[0];
    }
}

void EditorApplication::OnInit()
{
    auto renderSystem = ghost::Engine::GetInstance()->GetRenderSystem();
    renderSystem->SetClearColor(ghost::Color::Gray);

    _context.resourcesPath = ResolveResourcesPath();
    ghost::ResourceManager::GetInstance()->SetResourcesPath(_context.resourcesPath);
    ghost::LogManager::GetInstance()->LogInfo("Editor resources path: " + _context.resourcesPath);

    _context.sceneManager = new ghost::SceneManager();
    _context.sceneData = std::make_shared<ghost::Scene>();
    _context.sceneData->SetName("Scenes/editor_scene.gmap");
    _context.sceneManager->SetScene(_context.sceneData);

    const float width = static_cast<float>(GetWindow()->GetWidth());
    const float height = static_cast<float>(GetWindow()->GetHeight());
    _context.viewportAspect = height > 0.0f ? width / height : 1.0f;

    ghost::editor::CreateEditorCamera(_context);
    ghost::editor::CreateSceneCamera(_context);
    ghost::editor::SetupDefaultScene(_context);
    ghost::editor::RefreshModelAssets(_context);
    ghost::editor::RefreshSceneFiles(_context);

    _dockspacePanel = GHOST_MAKE_SMART_POINTER(ghost::editor::DockspacePanel, _context);
    _menuBarPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::MenuBarPanel, _context);
    _sceneViewportPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::SceneViewportPanel, _context);
    _viewportInputPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::ViewportInputPanel, _context);
    _hierarchyPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::HierarchyPanel, _context);
    _inspectorPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::InspectorPanel, _context);
    _contentBrowserPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::ContentBrowserPanel, _context);

    auto guiManager = ghost::GuiManager::GetInstance();
    guiManager->RegisterGuiPage(_dockspacePanel);
    guiManager->RegisterGuiPage(_menuBarPanel);
    guiManager->RegisterGuiPage(_sceneViewportPanel);
    guiManager->RegisterGuiPage(_viewportInputPanel);
    guiManager->RegisterGuiPage(_hierarchyPanel);
    guiManager->RegisterGuiPage(_inspectorPanel);
    guiManager->RegisterGuiPage(_contentBrowserPanel);
}

void EditorApplication::OnExit()
{
    delete _context.sceneManager;
    _context.sceneManager = nullptr;
    _context.camera = nullptr;
    _context.editorCamera = nullptr;
    _context.defaultLight = nullptr;
    _context.selectedNode = nullptr;
    _context.sceneData.reset();
}

void EditorApplication::OnUpdate()
{
    _context.SyncEditorCameraToSceneCamera();

    ghost::Camera* renderCamera = _context.GetRenderCamera();
    if (!_context.sceneManager || !renderCamera)
        return;

    _context.sceneManager->UpdateSceneGraph(renderCamera);
    _context.sceneManager->Render(renderCamera);
}

void EditorApplication::OnTick(float deltaTime)
{
    if (deltaTime <= 0.0f)
        return;

    char title[128] = { 0 };
    std::snprintf(title, sizeof(title), "GhostEditor FPS: %.2f", 1000.0f / deltaTime);
    GetWindow()->SetWindowTitle(title);
}
