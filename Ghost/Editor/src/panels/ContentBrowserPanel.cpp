#include "panels/ContentBrowserPanel.h"

#include "imgui.h"
#include "EditorScene.h"

namespace ghost
{
    namespace editor
    {
        void ContentBrowserPanel::Show()
        {
            const ImGuiIO& io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(8.0f, io.DisplaySize.y * 0.6f + 4.0f), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - 16.0f, io.DisplaySize.y * 0.4f - 12.0f), ImGuiCond_FirstUseEver);

            ImGui::Begin("Content Browser");

            ImGui::TextDisabled("Resource root: %s", _context.resourcesPath.c_str());
            ImGui::TextDisabled("Scene save/load lives in the File menu.");

            if (!_context.statusMessage.empty())
                ImGui::TextWrapped("%s", _context.statusMessage.c_str());

            ImGui::Separator();

            if (ImGui::Button("Refresh Models"))
                RefreshModelAssets(_context);
            ImGui::SameLine();
            ImGui::TextDisabled("%u assets", static_cast<unsigned>(_context.modelAssets.size()));

            ImGui::BeginChild("##ModelAssets", ImVec2(0.0f, 0.0f), true);

            if (_context.modelAssets.empty())
            {
                ImGui::TextDisabled("No model assets found. Check Resources/Meshes.");
            }
            else
            {
                const float addButtonWidth = ImGui::CalcTextSize("Add").x +
                    ImGui::GetStyle().FramePadding.x * 2.0f + 8.0f;

                for (size_t index = 0; index < _context.modelAssets.size(); ++index)
                {
                    const std::string& asset = _context.modelAssets[index];
                    ImGui::PushID(static_cast<int>(index));

                    const float selectableWidth = ImGui::GetContentRegionAvail().x - addButtonWidth;
                    if (ImGui::Selectable(asset.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick,
                        ImVec2(selectableWidth, 0.0f)))
                    {
                        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                            SpawnModel(_context, asset);
                    }

                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("Double-click to add %s to the scene", asset.c_str());

                    ImGui::SameLine();
                    if (ImGui::SmallButton("Add"))
                        SpawnModel(_context, asset);

                    ImGui::PopID();
                }
            }

            ImGui::EndChild();
            ImGui::End();
        }
    }
}
