#ifndef _EDITOR_APPLICATION_H_
#define _EDITOR_APPLICATION_H_

#include "Application.h"
#include "EditorContext.h"
#include "panels/ContentBrowserPanel.h"
#include "panels/HierarchyPanel.h"
#include "panels/InspectorPanel.h"
#include "panels/MenuBarPanel.h"
#include "panels/ViewportInputPanel.h"
#include "panels/DockspacePanel.h"
#include "panels/SceneViewportPanel.h"

class EditorApplication : public ghost::Application
{
public:
    EditorApplication() = default;
    virtual ~EditorApplication() = default;

    virtual void OnTick(float deltaTime) override;
    virtual void OnInit() override;
    virtual void OnExit() override;
    virtual void OnUpdate() override;

protected:
    ghost::editor::EditorContext _context;
    ghost::editor::DockspacePanelPtr _dockspacePanel;
    ghost::editor::MenuBarPanelPtr _menuBarPanel;
    ghost::editor::SceneViewportPanelPtr _sceneViewportPanel;
    ghost::editor::ViewportInputPanelPtr _viewportInputPanel;
    ghost::editor::HierarchyPanelPtr _hierarchyPanel;
    ghost::editor::InspectorPanelPtr _inspectorPanel;
    ghost::editor::ContentBrowserPanelPtr _contentBrowserPanel;
};

#endif
