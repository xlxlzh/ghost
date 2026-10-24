#include "panels/InspectorPanel.h"

#include <cstdio>

#include "imgui.h"
#include "ModelNode.h"
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

        void InspectorPanel::Show()
        {
            const ImGuiIO& io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 296.0f, 28.0f), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(288.0f, io.DisplaySize.y * 0.6f - 36.0f), ImGuiCond_FirstUseEver);

            ImGui::Begin("Inspector", 0, ImGuiWindowFlags_NoMove);

            SceneNode* node = _context.selectedNode;
            if (!node)
            {
                ImGui::TextDisabled("Select an object in the Hierarchy.");
                ImGui::End();
                return;
            }

            ImGui::PushID(static_cast<const void*>(node));

            char nameBuffer[128] = {};
            std::snprintf(nameBuffer, sizeof(nameBuffer), "%s", node->GetName().c_str());
            if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
                node->SetName(nameBuffer);

            ImGui::Text("Type: %s", GetNodeTypeName(node->GetType()));
            ImGui::Separator();

            Vector3f position;
            Vector3f rotation;
            Vector3f scale;
            node->GetTransform(position, rotation, scale);

            bool transformChanged = false;
            transformChanged |= ImGui::DragFloat3("Position", &position._x, 0.1f);
            transformChanged |= ImGui::DragFloat3("Rotation", &rotation._x, 0.5f);
            transformChanged |= ImGui::DragFloat3("Scale", &scale._x, 0.01f, 0.001f, 1000.0f);

            if (transformChanged)
                node->SetTransform(position, rotation, scale);

            if (Light* light = dynamic_cast<Light*>(node))
            {
                ImGui::Separator();

                static const char* lightTypeNames[] = { "Directional", "Point", "Spot" };
                int lightType = static_cast<int>(light->GetLightType());
                if (ImGui::Combo("Light Type", &lightType, lightTypeNames, IM_ARRAYSIZE(lightTypeNames)))
                    light->SetLightType(static_cast<LightType>(lightType));

                Color color = light->GetLightColor();
                if (ImGui::ColorEdit4("Color", color.GetColorPtr()))
                    light->SetLightColor(color);

                float shininess = light->GetLightShiness();
                if (ImGui::DragFloat("Shininess", &shininess, 0.5f, 0.0f, 1000.0f))
                    light->SetLightShiness(shininess);

                if (light->GetLightType() == LIGHT_POINT || light->GetLightType() == LIGHT_SPOT)
                {
                    float radius = light->GetRadius();
                    if (ImGui::DragFloat("Radius", &radius, 0.5f, 0.0f, 10000.0f))
                        light->SetRadius(radius);
                }

                if (light->GetLightType() == LIGHT_SPOT)
                {
                    float inner = light->GetSpotInner();
                    float outer = light->GetSpotOuter();
                    if (ImGui::DragFloat("Spot Inner", &inner, 0.5f, 0.0f, 89.0f))
                        light->SetSpotInner(inner);
                    if (ImGui::DragFloat("Spot Outer", &outer, 0.5f, 0.0f, 89.0f))
                        light->SetSpotOuter(outer);
                }

                if (light->GetLightType() == LIGHT_POINT || light->GetLightType() == LIGHT_SPOT)
                    ImGui::TextDisabled("Current renderer only uses the main directional light.");
                else if (light != _context.defaultLight)
                    ImGui::TextDisabled("Only the first directional light is used for rendering.");
            }
            else if (ModelNode* modelNode = dynamic_cast<ModelNode*>(node))
            {
                ImGui::Separator();

                const ModelPtr& model = modelNode->GetMesh();
                if (model)
                {
                    ImGui::Text("Model: %s", model->GetName().c_str());
                    ImGui::Text("SubMeshes: %u", model->GetSubMeshCount());
                }
                else
                {
                    ImGui::TextDisabled("No model assigned.");
                }
            }
            else if (Camera* camera = dynamic_cast<Camera*>(node))
            {
                ImGui::Separator();
                float nearPlane = camera->GetNear();
                if (ImGui::DragFloat("NearPlane", &nearPlane, 0.5f, 1.0f, 1000.0f))
                    camera->SetNear(nearPlane);
                float farPlane = camera->GetFar();
                if (ImGui::DragFloat("FarPlane", &farPlane, 0.5f, 1.0f, 1000.0f))
                    camera->SetFar(farPlane);
                ImGui::TextDisabled("Scene camera. The viewport is looking through it while selected.");
                ImGui::TextDisabled("Select another object (or navigate in the viewport) to return to the editor camera.");
            }

            if (_context.sceneManager)
                _context.sceneManager->SyncNodeToData(node);

            ImGui::Separator();

            const bool canDelete = (node != _context.camera && node != _context.editorCamera);
            if (!canDelete)
                ImGui::BeginDisabled();

            const bool deleteClicked = ImGui::Button("Delete Node");
            const bool deleteHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled);

            if (!canDelete)
            {
                ImGui::EndDisabled();
                if (deleteHovered)
                    ImGui::SetTooltip("The scene/editor camera cannot be deleted.");
            }

            ImGui::PopID();
            ImGui::End();

            if (deleteClicked)
                DeleteNode(_context, node);
        }
    }
}
