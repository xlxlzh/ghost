#include "panels/ContentBrowserPanel.h"

#include <cstdio>
#include <string>

#include "imgui.h"
#include "EditorScene.h"

namespace ghost
{
    namespace editor
    {
        namespace
        {
            std::string MakeDefaultModelPath(const std::string& source)
            {
                const size_t slashPos = source.find_last_of("/\\");
                std::string name = (slashPos == std::string::npos) ? source : source.substr(slashPos + 1);

                const size_t dotPos = name.find_last_of('.');
                if (dotPos != std::string::npos)
                    name = name.substr(0, dotPos);

                return "Meshes/" + name + ".gres";
            }
        }

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
            if (ImGui::Button("Import..."))
            {
                RefreshModelAssets(_context);
                _importSourceIndex = _context.sourceMeshAssets.empty() ? -1 : 0;
                if (_importSourceIndex >= 0)
                    std::snprintf(_importDestination, sizeof(_importDestination), "%s",
                        MakeDefaultModelPath(_context.sourceMeshAssets[0]).c_str());
                ImGui::OpenPopup("Import Model");
            }

            ImGui::SameLine();
            ImGui::TextDisabled("%u assets", static_cast<unsigned>(_context.modelAssets.size()));

            ImGui::BeginChild("##ModelAssets", ImVec2(0.0f, 0.0f), true);

            if (_context.modelAssets.empty())
            {
                ImGui::TextDisabled("No native models (.gres) found. Use Import... to create one.");
            }
            else
            {
                const float addButtonWidth = ImGui::CalcTextSize("Add").x +
                    ImGui::GetStyle().FramePadding.x * 2.0f + 8.0f;
                const float xmlButtonWidth = ImGui::CalcTextSize("XML").x +
                    ImGui::GetStyle().FramePadding.x * 2.0f + 8.0f;

                for (size_t index = 0; index < _context.modelAssets.size(); ++index)
                {
                    const std::string& asset = _context.modelAssets[index];
                    ImGui::PushID(static_cast<int>(index));

                    const float selectableWidth = ImGui::GetContentRegionAvail().x -
                        addButtonWidth - xmlButtonWidth;
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

                    ImGui::SameLine();
                    if (ImGui::SmallButton("XML"))
                        ExportModelAssetXml(_context, asset);

                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("Export debug XML next to this asset");

                    ImGui::PopID();
                }
            }

            ImGui::EndChild();

            if (ImGui::BeginPopupModal("Import Model", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::TextDisabled("Source meshes:");

                if (_context.sourceMeshAssets.empty())
                {
                    ImGui::TextDisabled("No .obj/.fbx source meshes found.");
                }
                else
                {
                    ImGui::BeginChild("##ImportSources", ImVec2(460.0f, 180.0f), true);
                    for (size_t index = 0; index < _context.sourceMeshAssets.size(); ++index)
                    {
                        const std::string& source = _context.sourceMeshAssets[index];
                        ImGui::PushID(static_cast<int>(index));
                        if (ImGui::Selectable(source.c_str(), _importSourceIndex == static_cast<int>(index)))
                        {
                            _importSourceIndex = static_cast<int>(index);
                            std::snprintf(_importDestination, sizeof(_importDestination), "%s",
                                MakeDefaultModelPath(source).c_str());
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndChild();
                }

                ImGui::SetNextItemWidth(460.0f);
                ImGui::InputText("Destination", _importDestination, sizeof(_importDestination));

                const bool canImport = _importSourceIndex >= 0 &&
                    _importSourceIndex < static_cast<int>(_context.sourceMeshAssets.size()) &&
                    _importDestination[0] != '\0';

                if (!canImport)
                    ImGui::BeginDisabled();

                if (ImGui::Button("Import"))
                {
                    const std::string source = _context.sourceMeshAssets[_importSourceIndex];
                    if (ImportModelAsset(_context, source, _importDestination))
                        ImGui::CloseCurrentPopup();
                }

                if (!canImport)
                    ImGui::EndDisabled();

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                    ImGui::CloseCurrentPopup();

                ImGui::SameLine();
                if (ImGui::Button("Refresh"))
                {
                    RefreshModelAssets(_context);
                    if (_importSourceIndex >= static_cast<int>(_context.sourceMeshAssets.size()))
                        _importSourceIndex = _context.sourceMeshAssets.empty() ? -1 : 0;
                }

                ImGui::EndPopup();
            }

            ImGui::End();
        }
    }
}
