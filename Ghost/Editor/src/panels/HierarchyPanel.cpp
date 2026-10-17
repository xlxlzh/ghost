#include "panels/HierarchyPanel.h"

#include "imgui.h"
#include "EditorScene.h"

namespace ghost
{
    namespace editor
    {
        namespace
        {
            const char* GetNodeTypeName(SceneNodeType type)
            {
                switch (type)
                {
                case SCENENODE_CAMERA: return "Camera";
                case SCENENODE_LIGHT:  return "Light";
                case SCENENODE_MODEL:  return "Model";
                default:               return "Node";
                }
            }
        }

        void HierarchyPanel::Show()
        {
            const ImGuiIO& io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(8.0f, 28.0f), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(280.0f, io.DisplaySize.y * 0.6f - 36.0f), ImGuiCond_FirstUseEver);

            ImGui::Begin("Hierarchy");

            if (!_context.scene || !_context.scene->GetRootNode())
            {
                ImGui::TextDisabled("No scene.");
                ImGui::End();
                return;
            }

            for (SceneNode* node : _context.scene->GetRootNode()->GetChildren())
                DrawNode(node);

            ImGui::End();
        }

        void HierarchyPanel::DrawNode(SceneNode* node)
        {
            if (!node || node->IsEditorOnly())
                return;

            const std::vector<SceneNode*>& children = node->GetChildren();

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                ImGuiTreeNodeFlags_DefaultOpen |
                ImGuiTreeNodeFlags_SpanAvailWidth;

            if (children.empty())
                flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

            if (node == _context.selectedNode)
                flags |= ImGuiTreeNodeFlags_Selected;

            const std::string& name = node->GetName();
            const char* displayName = name.empty() ? "Unnamed Node" : name.c_str();

            const bool open = ImGui::TreeNodeEx(static_cast<const void*>(node), flags,
                "%s  (%s)", displayName, GetNodeTypeName(node->GetType()));

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                FocusEditorCamera(_context, node);
            }
            else if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            {
                _context.selectedNode = node;
            }

            if (open && !children.empty())
            {
                for (SceneNode* child : children)
                    DrawNode(child);

                ImGui::TreePop();
            }
        }
    }
}
