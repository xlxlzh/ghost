#include "EditorScene.h"

#include <algorithm>
#include <filesystem>

#include "BinaryArchive.h"
#include "Engine.h"
#include "FileSystem.h"
#include "Resource.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "ModelNode.h"

namespace ghost
{
    namespace editor
    {
        namespace
        {
            const char* DefaultMaterialPath = "Materials/DefaultMaterial.gres";

            std::string GetFileStem(const std::string& path)
            {
                const size_t slashPos = path.find_last_of("/\\");
                std::string name = (slashPos == std::string::npos) ? path : path.substr(slashPos + 1);

                const size_t dotPos = name.find_last_of('.');
                if (dotPos != std::string::npos)
                    name = name.substr(0, dotPos);

                return name;
            }

            bool ContainsNode(SceneNode* root, SceneNode* target)
            {
                for (SceneNode* node = root; node != nullptr; node = node->GetParent())
                {
                    if (node == target)
                        return true;
                }

                return false;
            }
        }

        std::string MakeUniqueNodeName(const EditorContext& context, const std::string& baseName)
        {
            if (!context.sceneData)
                return baseName;

            if (!context.sceneData->FindNode(baseName))
                return baseName;

            for (unsigned index = 2; ; ++index)
            {
                const std::string candidate = baseName + " " + std::to_string(index);
                if (!context.sceneData->FindNode(candidate))
                    return candidate;
            }
        }

        Light* SpawnLight(EditorContext& context, LightType lightType)
        {
            if (!context.sceneManager || !context.sceneData)
                return nullptr;

            Vector3f position(0.0f, 50.0f, 8.0f);
            Vector3f rotation(0.0f, 0.0f, 0.0f);
            std::string baseName;
            float radius = 0.0f;
            float spotInner = 0.0f;
            float spotOuter = 0.0f;

            switch (lightType)
            {
            case LIGHT_DIRECTIONAL:
                baseName = "Directional Light";
                position = Vector3f(0.0f, 0.0f, 0.0f);
                rotation = Vector3f(0.0f, -45.0f, 45.0f);
                break;

            case LIGHT_POINT:
                baseName = "Point Light";
                radius = 50.0f;
                break;

            case LIGHT_SPOT:
                baseName = "Spot Light";
                radius = 100.0f;
                spotInner = 30.0f;
                spotOuter = 45.0f;
                rotation = Vector3f(45.0f, 0.0f, 0.0f);
                break;
            }

            SceneNodeData* data = context.sceneData->AddNode(
                SceneNodeDataType::Light, MakeUniqueNodeName(context, baseName));
            data->lightType = lightType;
            data->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
            data->shininess = 100.0f;
            data->radius = radius;
            data->spotInner = spotInner;
            data->spotOuter = spotOuter;
            data->position = position;
            data->rotation = rotation;
            data->scale = Vector3f(1.0f, 1.0f, 1.0f);

            Light* light = static_cast<Light*>(context.sceneManager->InstantiateNode(
                data, context.sceneManager->GetRootNode()));

            if (!light)
            {
                context.sceneData->RemoveNode(data);
                context.statusMessage = "Failed to create light.";
                return nullptr;
            }

            context.defaultLight = context.sceneManager->GetMainLigt();
            context.selectedNode = light;
            context.statusMessage = "Added " + light->GetName();

            return light;
        }

        ModelNode* SpawnModel(EditorContext& context, const std::string& logicalModelPath)
        {
            if (!context.sceneManager || !context.sceneData)
                return nullptr;

            static const float spawnOffsets[] = { -6.0f, -3.0f, 0.0f, 3.0f, 6.0f };
            const float x = spawnOffsets[context.spawnCounter % 5];
            ++context.spawnCounter;

            SceneNodeData* data = context.sceneData->AddNode(
                SceneNodeDataType::Model,
                MakeUniqueNodeName(context, "Model: " + GetFileStem(logicalModelPath)));
            data->modelPath = logicalModelPath;
            data->materialPath = DefaultMaterialPath;
            data->position = Vector3f(x, 40.0f, 8.0f);
            data->rotation = Vector3f(0.0f, 0.0f, 0.0f);
            data->scale = Vector3f(1.0f, 1.0f, 1.0f);

            ModelNode* node = static_cast<ModelNode*>(context.sceneManager->InstantiateNode(
                data, context.sceneManager->GetRootNode()));

            if (!node)
            {
                context.sceneData->RemoveNode(data);
                context.statusMessage = "Failed to create model node.";
                return nullptr;
            }

            context.selectedNode = node;

            if (node->GetMesh())
                context.statusMessage = "Added " + node->GetName();
            else
                context.statusMessage = "Added " + node->GetName() + " (model/material failed to load).";

            return node;
        }

        bool DeleteNode(EditorContext& context, SceneNode* node)
        {
            if (!context.sceneManager || !context.sceneData || !node)
                return false;

            if (node == context.camera || node == context.editorCamera)
                return false;

            SceneNodeData* data = node->GetSourceData();
            if (!data)
                return false;

            if (ContainsNode(context.camera, node))
                return false;

            if (ContainsNode(context.selectedNode, node))
                context.selectedNode = nullptr;

            context.sceneManager->RemoveNode(node);

            if (!context.sceneData->RemoveNode(data))
            {
                context.statusMessage = "Removed runtime node, but its scene data was not found.";
                context.defaultLight = context.sceneManager->GetMainLigt();
                return false;
            }

            context.defaultLight = context.sceneManager->GetMainLigt();
            context.statusMessage = "Deleted node.";

            return true;
        }

        void RenameNode(EditorContext& context, SceneNode* node, const std::string& newName)
        {
            if (!node || !context.sceneData)
                return;

            const size_t first = newName.find_first_not_of(" \t\r\n");
            if (first == std::string::npos)
            {
                context.statusMessage = "Node name cannot be empty.";
                return;
            }

            const size_t last = newName.find_last_not_of(" \t\r\n");
            const std::string trimmedName = newName.substr(first, last - first + 1);

            if (trimmedName.size() > 255)
            {
                context.statusMessage = "Node name is too long.";
                return;
            }

            if (trimmedName == node->GetName())
                return;

            SceneNodeData* data = node->GetSourceData();
            SceneNodeData* existing = context.sceneData->FindNode(trimmedName);
            if (existing && existing != data)
            {
                context.statusMessage = "A node named '" + trimmedName + "' already exists.";
                return;
            }

            node->SetName(trimmedName);
            context.statusMessage = "Renamed node to " + trimmedName;
        }

        bool ImportModelAsset(EditorContext& context, const std::string& sourceLogicalPath,
            const std::string& destLogicalPath)
        {
            if (context.resourcesPath.empty() || sourceLogicalPath.empty() || destLogicalPath.empty())
                return false;

            std::string destination = GetInternalPath(destLogicalPath);
            if (GetExtension(destination) != AssetFormat::GRES_EXTENSION)
                destination += AssetFormat::GRES_EXTENSION;

            if (destination.find('/') == std::string::npos)
                destination = "Meshes/" + destination;

            const std::string sourcePath = AddTrailingSlash(context.resourcesPath) + GetInternalPath(sourceLogicalPath);
            const std::string destPath = AddTrailingSlash(context.resourcesPath) + destination;

            const size_t slashPos = destPath.find_last_of('/');
            if (slashPos != std::string::npos)
            {
                std::error_code error;
                std::filesystem::create_directories(destPath.substr(0, slashPos), error);
            }

            ModelPtr model = ModelHelper::ImportModel(sourcePath);
            if (!model)
            {
                context.statusMessage = "Failed to import model: " + sourceLogicalPath;
                return false;
            }

            if (!ModelHelper::ExportModel(model, destPath))
            {
                context.statusMessage = "Failed to save native model: " + destination;
                return false;
            }

            RefreshModelAssets(context);
            context.statusMessage = "Imported " + sourceLogicalPath + " to " + destination;
            return true;
        }

        bool ExportModelAssetXml(EditorContext& context, const std::string& logicalModelPath)
        {
            if (context.resourcesPath.empty() || logicalModelPath.empty())
                return false;

            ModelPtr model = GHOST_SMARTPOINTER_CAST(Model,
                ResourceManager::GetInstance()->AddResource(
                    RESOURCE_MODEL, logicalModelPath, 0));

            if (!model)
            {
                context.statusMessage = "Failed to load model for XML export: " + logicalModelPath;
                return false;
            }

            std::string logicalXmlPath = GetInternalPath(logicalModelPath);
            const std::string extension = GetExtension(logicalXmlPath);
            if (!extension.empty())
                logicalXmlPath.erase(logicalXmlPath.size() - extension.size());
            logicalXmlPath += AssetFormat::XML_EXTENSION;

            const std::string xmlPath = AddTrailingSlash(context.resourcesPath) + logicalXmlPath;
            if (!ModelHelper::ExportModelXml(model, xmlPath))
            {
                context.statusMessage = "Failed to export model XML: " + logicalXmlPath;
                return false;
            }

            context.statusMessage = "Exported model XML: " + logicalXmlPath;
            return true;
        }

        void RefreshModelAssets(EditorContext& context)
        {
            context.modelAssets.clear();
            context.sourceMeshAssets.clear();

            if (context.resourcesPath.empty())
            {
                context.statusMessage = "Resources path is empty.";
                return;
            }

            std::vector<std::string> files;
            Engine::GetInstance()->GetFileSystem()->ScanDir(files,
                AddTrailingSlash(context.resourcesPath) + "Meshes", "*");

            for (const std::string& file : files)
            {
                const std::string extension = GetExtension(file);
                if (extension == AssetFormat::GRES_EXTENSION)
                    context.modelAssets.push_back("Meshes/" + file);
                else if (extension == ".obj" || extension == ".fbx")
                    context.sourceMeshAssets.push_back("Meshes/" + file);
            }

            std::sort(context.modelAssets.begin(), context.modelAssets.end());
            std::sort(context.sourceMeshAssets.begin(), context.sourceMeshAssets.end());

            if (context.modelAssets.empty())
            {
                context.statusMessage = "No native models (.gres) found under " +
                    AddTrailingSlash(context.resourcesPath) + "Meshes.";
            }
        }

        Camera* CreateSceneCamera(EditorContext& context)
        {
            if (!context.sceneManager || !context.sceneData)
                return nullptr;

            SceneNodeData* data = context.sceneData->AddNode(
                SceneNodeDataType::Camera, "Main Camera");
            data->position = Vector3f(0.0f, 40.0f, 0.0f);
            data->rotation = Vector3f(0.0f, 0.0f, 0.0f);
            data->scale = Vector3f(1.0f, 1.0f, 1.0f);
            data->cameraFov = 90.0f;
            data->cameraAspect = context.viewportAspect > 0.0f ? context.viewportAspect : 1.0f;
            data->cameraNear = 1.0f;
            data->cameraFar = 100.0f;

            Camera* camera = static_cast<Camera*>(context.sceneManager->InstantiateNode(
                data, context.sceneManager->GetRootNode()));

            context.camera = camera;
            return camera;
        }

        Camera* CreateEditorCamera(EditorContext& context)
        {
            if (!context.sceneManager)
                return nullptr;

            Camera* camera = new Camera(context.sceneManager);
            const float aspect = context.viewportAspect > 0.0f ? context.viewportAspect : 1.0f;
            camera->SetProjectParams(90.0f, aspect, 1.0f, 1000.0f);
            camera->SetName("Editor Camera");
            camera->SetTransform(Vector3f(0.0f, 40.0f, 0.0f),
                Vector3f(0.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 1.0f, 1.0f));
            camera->SetEditorOnly(true);

            context.sceneManager->AddNodeToRoot(camera);
            context.editorCamera = camera;

            return camera;
        }

        void SetupDefaultScene(EditorContext& context)
        {
            if (!context.sceneManager || !context.sceneData)
                return;

            Light* mainLight = SpawnLight(context, LIGHT_DIRECTIONAL);

            ModelNode* ground = SpawnModel(context, "Meshes/Ground.gres");
            if (ground)
            {
                ground->SetName("Ground");
                ground->SetTransform(Vector3f(-150.0f, 30.0f, -150.0f),
                    Vector3f(0.0f, 0.0f, 0.0f),
                    Vector3f(300.0f, 1.0f, 300.0f));

                if (ground->GetMesh())
                    context.statusMessage = "Default scene ready.";
                else
                    context.statusMessage = "Default scene created, but the ground model failed to load.";
            }

            context.defaultLight = context.sceneManager->GetMainLigt();
            if (mainLight)
                context.selectedNode = mainLight;
            else if (context.defaultLight)
                context.selectedNode = context.defaultLight;
        }

        void FocusEditorCamera(EditorContext& context, SceneNode* node)
        {
            if (!node)
                return;

            context.selectedNode = node;

            if (!context.editorCamera || node == context.camera || node == context.editorCamera)
                return;

            const Matrix4x4f& nodeAbs = node->GetAbsTrans();
            const Vector3f target(nodeAbs._41, nodeAbs._42, nodeAbs._43);

            Vector3f position;
            Vector3f rotation;
            Vector3f scale;
            context.editorCamera->GetTransform(position, rotation, scale);

            const Matrix4x4f& cameraAbs = context.editorCamera->GetAbsTrans();
            Vector3f forward(cameraAbs._31, cameraAbs._32, cameraAbs._33);

            const float length = forward.Length();
            if (length < 0.0001f)
                return;

            forward /= length;
            context.editorCamera->SetTransform(target - forward * 15.0f, rotation, scale);
        }

        void NewScene(EditorContext& context)
        {
            if (!context.sceneManager)
                return;

            Vector3f editorPosition(0.0f, 40.0f, 0.0f);
            Vector3f editorRotation(0.0f, 0.0f, 0.0f);
            Vector3f editorScale(1.0f, 1.0f, 1.0f);
            if (context.editorCamera)
                context.editorCamera->GetTransform(editorPosition, editorRotation, editorScale);

            context.camera = nullptr;
            context.editorCamera = nullptr;
            context.defaultLight = nullptr;
            context.selectedNode = nullptr;
            context.spawnCounter = 0;

            ScenePtr scene = std::make_shared<Scene>();
            scene->SetName("Scenes/untitled.gmap");
            context.sceneManager->SetScene(scene);
            context.sceneData = scene;
            context.scenePath = "Scenes/untitled.gmap";

            Camera* editorCamera = CreateEditorCamera(context);
            if (editorCamera)
                editorCamera->SetTransform(editorPosition, editorRotation, editorScale);

            CreateSceneCamera(context);
            SetupDefaultScene(context);
            context.statusMessage = "New scene created.";
        }
    }
}
