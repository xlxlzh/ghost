#include "EditorApplication.h"

#include <cstdio>
#include <string>

#include "Engine.h"
#include "FileSystem.h"
#include "GUIManager.h"
#include "LogManager.h"
#include "Resource.h"
#include "EditorScene.h"
#include "SceneSerializer.h"

namespace
{
    std::string ResolveResourcesPath()
    {
        auto fileSystem = ghost::Engine::GetInstance()->GetFileSystem();
        const std::string currentDir = ghost::AddTrailingSlash(fileSystem->GetCurrentDir());

        // Depending on whether the editor is launched from the Visual Studio
        // project directory or straight from Build/Bin, the resources root is
        // at a different relative depth.
        const std::string candidates[] =
        {
            currentDir + "../../Resources/",     // Build/Ghost/Editor
            currentDir + "../Resources/",        // Build/Bin
            currentDir + "../../../Resources/",  // SampleBase convention
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

    _context.scene = new ghost::SceneManager();

    const float width = static_cast<float>(GetWindow()->GetWidth());
    const float height = static_cast<float>(GetWindow()->GetHeight());
    _context.viewportAspect = height > 0.0f ? width / height : 1.0f;

    ghost::editor::CreateEditorCamera(_context);
    ghost::editor::CreateSceneCamera(_context);
    ghost::editor::SetupDefaultScene(_context);
    ghost::editor::RefreshModelAssets(_context);
    ghost::editor::RefreshSceneFiles(_context);

    _menuBarPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::MenuBarPanel, _context);
    _viewportInputPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::ViewportInputPanel, _context);
    _hierarchyPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::HierarchyPanel, _context);
    _inspectorPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::InspectorPanel, _context);
    _contentBrowserPanel = GHOST_MAKE_SMART_POINTER(ghost::editor::ContentBrowserPanel, _context);

    auto guiManager = ghost::GuiManager::GetInstance();
    // The menu bar must be registered first so it is drawn before the other
    // panels (and so scene changes happen before they read the hierarchy).
    guiManager->RegisterGuiPage(_menuBarPanel);
    guiManager->RegisterGuiPage(_viewportInputPanel);
    guiManager->RegisterGuiPage(_hierarchyPanel);
    guiManager->RegisterGuiPage(_inspectorPanel);
    guiManager->RegisterGuiPage(_contentBrowserPanel);
}

void EditorApplication::OnExit()
{
    delete _context.scene;
    _context.scene = nullptr;
    _context.camera = nullptr;
    _context.defaultLight = nullptr;
    _context.selectedNode = nullptr;
}

void EditorApplication::OnUpdate()
{
    _context.SyncEditorCameraToSceneCamera();

    ghost::Camera* renderCamera = _context.GetRenderCamera();
    if (!_context.scene || !renderCamera)
        return;

    // SceneManager::Render owns BeginScene/RenderAllPages/EndScene.
    _context.scene->UpdateSceneGraph(renderCamera);
    _context.scene->Render(renderCamera);
}

void EditorApplication::OnTick(float deltaTime)
{
    if (deltaTime <= 0.0f)
        return;

    char title[128] = { 0 };
    std::snprintf(title, sizeof(title), "GhostEditor FPS: %.2f", 1000.0f / deltaTime);
    GetWindow()->SetWindowTitle(title);
}
