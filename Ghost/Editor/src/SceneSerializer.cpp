#include "SceneSerializer.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <sstream>

#include "tinyxml2.h"

#include "Engine.h"
#include "FileSystem.h"
#include "ModelNode.h"
#include "EditorScene.h"

namespace ghost
{
    namespace editor
    {
        namespace
        {
            std::string FormatVector(const Vector3f& value)
            {
                char buffer[128] = {};
                std::snprintf(buffer, sizeof(buffer), "%g %g %g",
                    value._x, value._y, value._z);
                return buffer;
            }

            std::string FormatColor(const Color& color)
            {
                char buffer[160] = {};
                std::snprintf(buffer, sizeof(buffer), "%g %g %g %g",
                    color._r, color._g, color._b, color._a);
                return buffer;
            }

            Vector3f ParseVector(const char* text, const Vector3f& fallback)
            {
                if (!text)
                    return fallback;

                std::istringstream stream(text);
                Vector3f value = fallback;
                if (!(stream >> value._x >> value._y >> value._z))
                    return fallback;

                return value;
            }

            Color ParseColor(const char* text, const Color& fallback)
            {
                if (!text)
                    return fallback;

                std::istringstream stream(text);
                Color value = fallback;
                if (!(stream >> value._r >> value._g >> value._b >> value._a))
                    return fallback;

                return value;
            }

            std::string NormalizeScenePath(const std::string& path)
            {
                std::string result = path;
                if (result.empty())
                    result = "editor_scene";

                if (GetExtension(result).empty())
                    result += ".xml";

                if (result.find('/') == std::string::npos &&
                    result.find('\\') == std::string::npos)
                {
                    result = "Scenes/" + result;
                }

                return GetInternalPath(result);
            }

            void EnsureDirectory(const std::string& filePath)
            {
                const size_t slashPos = filePath.find_last_of("/\\");
                if (slashPos == std::string::npos)
                    return;

                const std::string directory = filePath.substr(0, slashPos);
                std::error_code error;
                if (!std::filesystem::exists(directory, error))
                    std::filesystem::create_directories(directory, error);
            }

            std::string GetNodeName(const tinyxml2::XMLElement& element, const std::string& fallback)
            {
                const char* name = element.Attribute("name");
                return (name && name[0]) ? std::string(name) : fallback;
            }
        }

        bool SaveScene(EditorContext& context, const std::string& relativePath)
        {
            if (!context.scene || !context.scene->GetRootNode())
            {
                context.statusMessage = "Cannot save scene: no scene.";
                return false;
            }

            const std::string normalizedPath = NormalizeScenePath(relativePath);
            const std::string fullPath = AddTrailingSlash(context.resourcesPath) + normalizedPath;
            EnsureDirectory(fullPath);

            tinyxml2::XMLDocument document;
            document.InsertFirstChild(document.NewDeclaration());

            tinyxml2::XMLElement* sceneElement = document.NewElement("Scene");
            sceneElement->SetAttribute("version", 1);
            document.InsertEndChild(sceneElement);

            for (SceneNode* node : context.scene->GetRootNode()->GetChildren())
            {
                if (!node || node->IsEditorOnly())
                    continue;

                Vector3f position;
                Vector3f rotation;
                Vector3f scale;
                node->GetTransform(position, rotation, scale);

                tinyxml2::XMLElement* nodeElement = document.NewElement("Node");
                nodeElement->SetAttribute("name", node->GetName().c_str());
                nodeElement->SetAttribute("position", FormatVector(position).c_str());
                nodeElement->SetAttribute("rotation", FormatVector(rotation).c_str());
                nodeElement->SetAttribute("scale", FormatVector(scale).c_str());

                if (dynamic_cast<Camera*>(node))
                {
                    nodeElement->SetAttribute("type", "Camera");
                }
                else if (Light* light = dynamic_cast<Light*>(node))
                {
                    nodeElement->SetAttribute("type", "Light");
                    nodeElement->SetAttribute("lightType", static_cast<int>(light->GetLightType()));
                    nodeElement->SetAttribute("color", FormatColor(light->GetLightColor()).c_str());
                    nodeElement->SetAttribute("shininess", light->GetLightShiness());
                    nodeElement->SetAttribute("radius", light->GetRadius());
                    nodeElement->SetAttribute("spotInner", light->GetSpotInner());
                    nodeElement->SetAttribute("spotOuter", light->GetSpotOuter());
                }
                else if (ModelNode* modelNode = dynamic_cast<ModelNode*>(node))
                {
                    const ModelPtr& model = modelNode->GetMesh();
                    if (!model)
                        continue;

                    nodeElement->SetAttribute("type", "Model");
                    nodeElement->SetAttribute("model", model->GetName().c_str());
                }
                else
                {
                    continue;
                }

                sceneElement->InsertEndChild(nodeElement);
            }

            if (document.SaveFile(fullPath.c_str()) != tinyxml2::XML_SUCCESS)
            {
                context.statusMessage = "Failed to save scene: " + normalizedPath;
                return false;
            }

            context.scenePath = normalizedPath;
            context.statusMessage = "Saved scene: " + normalizedPath;
            return true;
        }

        bool LoadScene(EditorContext& context, const std::string& relativePath)
        {
            if (!context.scene)
            {
                context.statusMessage = "Cannot load scene: no scene.";
                return false;
            }

            const std::string normalizedPath = NormalizeScenePath(relativePath);
            const std::string fullPath = AddTrailingSlash(context.resourcesPath) + normalizedPath;

            tinyxml2::XMLDocument document;
            if (document.LoadFile(fullPath.c_str()) != tinyxml2::XML_SUCCESS)
            {
                context.statusMessage = "Failed to load scene: " + normalizedPath;
                return false;
            }

            tinyxml2::XMLElement* sceneElement = document.FirstChildElement("Scene");
            if (!sceneElement)
            {
                context.statusMessage = "Invalid scene file: " + normalizedPath;
                return false;
            }

            // Keep the current editor view across scene loads.
            Vector3f editorPosition(0.0f, 40.0f, 0.0f);
            Vector3f editorRotation(0.0f, 0.0f, 0.0f);
            Vector3f editorScale(1.0f, 1.0f, 1.0f);
            if (context.editorCamera)
                context.editorCamera->GetTransform(editorPosition, editorRotation, editorScale);

            // Only clear once the file has been validated.
            context.scene->Clear();
            context.camera = nullptr;
            context.editorCamera = nullptr;
            context.defaultLight = nullptr;
            context.selectedNode = nullptr;
            context.spawnCounter = 0;

            Camera* loadedCamera = nullptr;
            SceneNode* firstSelectableNode = nullptr;
            unsigned loadedNodes = 0;
            unsigned failedNodes = 0;

            Camera* editorCamera = CreateEditorCamera(context);
            if (editorCamera)
                editorCamera->SetTransform(editorPosition, editorRotation, editorScale);

            for (tinyxml2::XMLElement* nodeElement = sceneElement->FirstChildElement("Node");
                nodeElement != nullptr;
                nodeElement = nodeElement->NextSiblingElement("Node"))
            {
                const char* type = nodeElement->Attribute("type");
                if (!type)
                    continue;

                const Vector3f position = ParseVector(nodeElement->Attribute("position"), Vector3f(0.0f, 0.0f, 0.0f));
                const Vector3f rotation = ParseVector(nodeElement->Attribute("rotation"), Vector3f(0.0f, 0.0f, 0.0f));
                const Vector3f scale = ParseVector(nodeElement->Attribute("scale"), Vector3f(1.0f, 1.0f, 1.0f));

                if (std::string(type) == "Camera")
                {
                    Camera* camera = new Camera(context.scene);
                    camera->SetProjectParams(90.0f, context.viewportAspect > 0.0f ? context.viewportAspect : 1.0f, 1.0f, 100.0f);
                    camera->SetName(GetNodeName(*nodeElement, "Main Camera"));
                    camera->SetTransform(position, rotation, scale);
                    context.scene->AddNodeToRoot(camera);

                    loadedCamera = camera;
                    ++loadedNodes;
                }
                else if (std::string(type) == "Light")
                {
                    int lightType = nodeElement->IntAttribute("lightType", 0);
                    if (lightType < 0 || lightType > LIGHT_SPOT)
                        lightType = LIGHT_DIRECTIONAL;

                    Light* light = SpawnLight(context, static_cast<LightType>(lightType));
                    if (!light)
                    {
                        ++failedNodes;
                        continue;
                    }

                    light->SetName(GetNodeName(*nodeElement, light->GetName()));
                    light->SetTransform(position, rotation, scale);
                    light->SetLightColor(ParseColor(nodeElement->Attribute("color"), Color::White));
                    light->SetLightShiness(nodeElement->FloatAttribute("shininess", 100.0f));
                    light->SetRadius(nodeElement->FloatAttribute("radius", 0.0f));
                    light->SetSpotInner(nodeElement->FloatAttribute("spotInner", 0.0f));
                    light->SetSpotOuter(nodeElement->FloatAttribute("spotOuter", 0.0f));

                    if (!firstSelectableNode)
                        firstSelectableNode = light;

                    ++loadedNodes;
                }
                else if (std::string(type) == "Model")
                {
                    const char* modelPath = nodeElement->Attribute("model");
                    if (!modelPath || !modelPath[0])
                    {
                        ++failedNodes;
                        continue;
                    }

                    ModelNode* modelNode = SpawnModel(context, modelPath);
                    if (!modelNode)
                    {
                        ++failedNodes;
                        continue;
                    }

                    modelNode->SetName(GetNodeName(*nodeElement, modelNode->GetName()));
                    modelNode->SetTransform(position, rotation, scale);

                    if (!firstSelectableNode)
                        firstSelectableNode = modelNode;

                    ++loadedNodes;
                }
            }

            if (!loadedCamera)
                loadedCamera = CreateSceneCamera(context);

            context.camera = loadedCamera;
            context.defaultLight = context.scene->GetMainLigt();
            context.selectedNode = firstSelectableNode;
            context.scenePath = normalizedPath;

            context.statusMessage = "Loaded scene: " + normalizedPath +
                " (" + std::to_string(loadedNodes) + " nodes";
            if (failedNodes > 0)
                context.statusMessage += ", " + std::to_string(failedNodes) + " failed";
            context.statusMessage += ")";

            return true;
        }

        void RefreshSceneFiles(EditorContext& context)
        {
            context.sceneFiles.clear();

            if (context.resourcesPath.empty())
                return;

            std::vector<std::string> files;
            Engine::GetInstance()->GetFileSystem()->ScanDir(files,
                AddTrailingSlash(context.resourcesPath) + "Scenes", "*");

            for (const std::string& file : files)
            {
                if (GetExtension(file) == ".xml")
                    context.sceneFiles.push_back("Scenes/" + file);
            }

            std::sort(context.sceneFiles.begin(), context.sceneFiles.end());
        }
    }
}
