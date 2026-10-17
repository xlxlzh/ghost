#include "panels/MenuBarPanel.h"

#include <cstdio>

#include "imgui.h"
#include "EditorScene.h"
#include "SceneSerializer.h"

namespace ghost
{
    namespace editor
    {
        void MenuBarPanel::Show()
        {
            bool requestSaveAs = false;
            bool requestOpen = false;

            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("New Scene"))
                        NewScene(_context);

                    ImGui::Separator();

                    if (ImGui::MenuItem("Open Scene..."))
                        requestOpen = true;

                    if (ImGui::MenuItem("Save Scene"))
                    {
                        if (_context.scenePath.empty())
                        {
                            requestSaveAs = true;
                        }
                        else
                        {
                            SaveScene(_context, _context.scenePath);
                            RefreshSceneFiles(_context);
                        }
                    }

                    if (ImGui::MenuItem("Save Scene As..."))
                        requestSaveAs = true;

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Light"))
                {
                    if (ImGui::MenuItem("Directional Light"))
                        SpawnLight(_context, LIGHT_DIRECTIONAL);
                    if (ImGui::MenuItem("Point Light"))
                        SpawnLight(_context, LIGHT_POINT);
                    if (ImGui::MenuItem("Spot Light"))
                        SpawnLight(_context, LIGHT_SPOT);

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Model"))
                {
                    if (_context.modelAssets.empty())
                    {
                        ImGui::MenuItem("No model assets", nullptr, false, false);
                    }
                    else
                    {
                        for (const std::string& asset : _context.modelAssets)
                        {
                            if (ImGui::MenuItem(asset.c_str()))
                                SpawnModel(_context, asset);
                        }
                    }

                    ImGui::EndMenu();
                }

                ImGui::SameLine();
                ImGui::TextDisabled("Scene: %s", _context.scenePath.c_str());

                ImGui::EndMainMenuBar();
            }

            // OpenPopup/BeginPopupModal must share the same window ID stack.
            // GuiPages are drawn outside of any ImGui window, so use a tiny
            // invisible host window for the scene dialogs.
            const ImGuiWindowFlags hostFlags =
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav;

            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImVec2(1.0f, 1.0f));
            ImGui::Begin("##EditorDialogHost", nullptr, hostFlags);

            if (requestSaveAs)
            {
                std::snprintf(_dialogPath, sizeof(_dialogPath), "%s", _context.scenePath.c_str());
                ImGui::OpenPopup("Save Scene As");
            }

            if (requestOpen)
            {
                RefreshSceneFiles(_context);
                std::snprintf(_dialogPath, sizeof(_dialogPath), "%s", _context.scenePath.c_str());
                ImGui::OpenPopup("Open Scene");
            }

            if (ImGui::BeginPopupModal("Save Scene As", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::TextDisabled("Resources: %s", _context.resourcesPath.c_str());
                ImGui::SetNextItemWidth(460.0f);
                if (ImGui::IsWindowAppearing())
                    ImGui::SetKeyboardFocusHere();
                ImGui::InputText("Path", _dialogPath, sizeof(_dialogPath));

                if (ImGui::Button("Save"))
                {
                    if (SaveScene(_context, _dialogPath))
                    {
                        RefreshSceneFiles(_context);
                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                    ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
            }

            if (ImGui::BeginPopupModal("Open Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::TextDisabled("Resources: %s", _context.resourcesPath.c_str());
                ImGui::SetNextItemWidth(460.0f);
                if (ImGui::IsWindowAppearing())
                    ImGui::SetKeyboardFocusHere();
                ImGui::InputText("Path", _dialogPath, sizeof(_dialogPath));

                if (ImGui::Button("Open"))
                {
                    if (LoadScene(_context, _dialogPath))
                    {
                        RefreshSceneFiles(_context);
                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                    ImGui::CloseCurrentPopup();

                ImGui::SameLine();
                if (ImGui::Button("Refresh"))
                    RefreshSceneFiles(_context);

                ImGui::Separator();

                if (_context.sceneFiles.empty())
                {
                    ImGui::TextDisabled("No scene files. Save a scene first.");
                }
                else
                {
                    ImGui::BeginChild("##SceneList", ImVec2(460.0f, 180.0f), true);
                    for (size_t index = 0; index < _context.sceneFiles.size(); ++index)
                    {
                        const std::string& sceneFile = _context.sceneFiles[index];
                        ImGui::PushID(static_cast<int>(index));

                        if (ImGui::Selectable(sceneFile.c_str(), _context.scenePath == sceneFile,
                            ImGuiSelectableFlags_AllowDoubleClick))
                        {
                            std::snprintf(_dialogPath, sizeof(_dialogPath), "%s", sceneFile.c_str());
                            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                            {
                                if (LoadScene(_context, sceneFile))
                                {
                                    RefreshSceneFiles(_context);
                                    ImGui::CloseCurrentPopup();
                                }
                            }
                        }

                        if (ImGui::IsItemHovered())
                            ImGui::SetTooltip("Double-click to open %s", sceneFile.c_str());

                        ImGui::PopID();
                    }
                    ImGui::EndChild();
                }

                ImGui::EndPopup();
            }

            ImGui::End();
        }
    }
}
