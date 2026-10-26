#include "panels/DockspacePanel.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace ghost
{
    namespace editor
    {
        void DockspacePanel::Show()
        {
            const ImGuiID dockspaceId = ImGui::GetID("EditorDockspace");
            if (!_layoutInitialized)
            {
                _layoutInitialized = true;
                
                ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);
                
                ImGuiID center = dockspaceId;
                ImGuiID left = ImGui::DockBuilderSplitNode(center, ImGuiDir_Left, 0.2f, nullptr, &center);
                ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.25f, nullptr, &center);
                ImGuiID bottom = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.3f, nullptr, &center);
                
                ImGui::DockBuilderDockWindow("Hierarchy", left);
                ImGui::DockBuilderDockWindow("Inspector", right);
                ImGui::DockBuilderDockWindow("Content Browser", bottom);
                ImGui::DockBuilderDockWindow("SceneViewport", center);
                
                ImGui::DockBuilderFinish(dockspaceId);
            }
            
            ImGui::DockSpaceOverViewport(dockspaceId, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
        }
    }
}
