#include "EditorScene.h"

#include <algorithm>

#include "Engine.h"
#include "FileSystem.h"
#include "LogManager.h"
#include "Resource.h"
#include "Material.h"

namespace ghost
{
    namespace editor
    {
        namespace
        {
            std::string GetFileStem(const std::string& path)
            {
                const size_t slashPos = path.find_last_of("/\\");
                std::string name = (slashPos == std::string::npos) ? path : path.substr(slashPos + 1);

                const size_t dotPos = name.find_last_of('.');
                if (dotPos != std::string::npos)
                    name = name.substr(0, dotPos);

                return name;
            }
        }

        std::string MakeUniqueNodeName(const EditorContext& context, const std::string& baseName)
        {
            if (!context.scene)
                return baseName;

            auto nameExists = [&context](const std::string& name)
            {
                for (SceneNode* node : context.scene->GetSceneNodes())
                {
                    if (node && node->GetName() == name)
                        return true;
                }

                return false;
            };

            if (!nameExists(baseName))
                return baseName;

            for (unsigned index = 2; ; ++index)
            {
                const std::string candidate = baseName + " " + std::to_string(index);
                if (!nameExists(candidate))
                    return candidate;
            }
        }

        Light* SpawnLight(EditorContext& context, LightType lightType)
        {
            if (!context.scene)
                return nullptr;

            Light* light = new Light(context.scene);
            light->SetLightType(lightType);
            light->SetLightColor(Color::White);
            light->SetLightShiness(100.0f);

            Vector3f position(0.0f, 50.0f, 8.0f);
            Vector3f rotation(0.0f, 0.0f, 0.0f);
            std::string baseName;

            switch (lightType)
            {
            case LIGHT_DIRECTIONAL:
                baseName = "Directional Light";
                position = Vector3f(0.0f, 0.0f, 0.0f);
                rotation = Vector3f(0.0f, -45.0f, 45.0f);
                break;

            case LIGHT_POINT:
                baseName = "Point Light";
                light->SetRadius(50.0f);
                break;

            case LIGHT_SPOT:
                baseName = "Spot Light";
                light->SetRadius(100.0f);
                light->SetSpotInner(30.0f);
                light->SetSpotOuter(45.0f);
                rotation = Vector3f(45.0f, 0.0f, 0.0f);
                break;
            }

            light->SetName(MakeUniqueNodeName(context, baseName));
            light->SetTransform(position, rotation, Vector3f(1.0f, 1.0f, 1.0f));

            context.scene->AddNodeToRoot(light);

            if (!context.defaultLight)
                context.defaultLight = light;

            context.selectedNode = light;
            context.statusMessage = "Added " + light->GetName();

            return light;
        }

        ModelNode* SpawnModel(EditorContext& context, const std::string& logicalModelPath)
        {
            if (!context.scene)
                return nullptr;

            if (!context.defaultMaterial)
            {
                context.statusMessage = "Cannot add model: default material is not loaded.";
                return nullptr;
            }

            ModelPtr model = GHOST_SMARTPOINTER_CAST(Model,
                ResourceManager::GetInstance()->AddResource(RESOURCE_MODEL, logicalModelPath, 0));

            if (!model)
            {
                context.statusMessage = "Failed to load model: " + logicalModelPath;
                return nullptr;
            }

            const unsigned subMeshCount = model->GetSubMeshCount();
            if (subMeshCount == 0)
            {
                context.statusMessage = "Model has no submeshes: " + logicalModelPath;
                return nullptr;
            }

            // ModelNode renders every submesh and RenderSystem::SetMaterial does
            // not accept null, so every submesh needs a material.
            for (unsigned index = 0; index < subMeshCount; ++index)
                model->SetMaterial(index, context.defaultMaterial);

            ModelNode* node = new ModelNode(context.scene);
            node->SetModel(model);
            node->SetName(MakeUniqueNodeName(context, "Model: " + GetFileStem(logicalModelPath)));

            static const float spawnOffsets[] = { -6.0f, -3.0f, 0.0f, 3.0f, 6.0f };
            const float x = spawnOffsets[context.spawnCounter % 5];
            ++context.spawnCounter;

            node->SetTransform(Vector3f(x, 40.0f, 8.0f),
                Vector3f(0.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 1.0f, 1.0f));

            context.scene->AddNodeToRoot(node);

            context.selectedNode = node;
            context.statusMessage = "Added " + node->GetName();

            return node;
        }

        bool DeleteNode(EditorContext& context, SceneNode* node)
        {
            if (!context.scene || !node || node == context.camera || node == context.editorCamera)
                return false;

            if (context.selectedNode == node)
                context.selectedNode = nullptr;

            context.scene->RemoveNode(node);
            context.defaultLight = context.scene->GetMainLigt();
            context.statusMessage = "Deleted node.";

            return true;
        }

        void RefreshModelAssets(EditorContext& context)
        {
            context.modelAssets.clear();

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
                if (extension == ".obj" || extension == ".fbx")
                    context.modelAssets.push_back("Meshes/" + file);
            }

            std::sort(context.modelAssets.begin(), context.modelAssets.end());

            if (context.modelAssets.empty())
            {
                context.statusMessage = "No model assets found under " +
                    AddTrailingSlash(context.resourcesPath) + "Meshes.";
            }
        }

        Camera* CreateSceneCamera(EditorContext& context)
        {
            if (!context.scene)
                return nullptr;

            Camera* camera = new Camera(context.scene);
            const float aspect = context.viewportAspect > 0.0f ? context.viewportAspect : 1.0f;
            camera->SetProjectParams(90.0f, aspect, 1.0f, 100.0f);
            camera->SetName("Main Camera");
            camera->SetTransform(Vector3f(0.0f, 40.0f, 0.0f),
                Vector3f(0.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 1.0f, 1.0f));

            context.scene->AddNodeToRoot(camera);
            context.camera = camera;

            return camera;
        }

        Camera* CreateEditorCamera(EditorContext& context)
        {
            if (!context.scene)
                return nullptr;

            Camera* camera = new Camera(context.scene);
            const float aspect = context.viewportAspect > 0.0f ? context.viewportAspect : 1.0f;
            camera->SetProjectParams(90.0f, aspect, 1.0f, 100.0f);
            camera->SetName("Editor Camera");
            camera->SetTransform(Vector3f(0.0f, 40.0f, 0.0f),
                Vector3f(0.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 1.0f, 1.0f));
            camera->SetEditorOnly(true);

            context.scene->AddNodeToRoot(camera);
            context.editorCamera = camera;

            return camera;
        }

        void SetupDefaultScene(EditorContext& context)
        {
            if (!context.scene)
                return;

            ResourcePtr materialResource = ResourceManager::GetInstance()->AddResource(
                RESOURCE_MATERIAL, "Materials/DefaultMaterial.xml", 0);
            context.defaultMaterial = GHOST_SMARTPOINTER_CAST(Material, materialResource);

            if (!context.defaultMaterial)
                context.statusMessage = "Failed to load Materials/DefaultMaterial.xml";

            Light* mainLight = SpawnLight(context, LIGHT_DIRECTIONAL);

            ModelNode* ground = SpawnModel(context, "Meshes/cube.obj");
            if (ground)
            {
                ground->SetName("Ground");
                // cube.obj is authored from (0,0,0) to (1,1,1), so offset by
                // half its scaled size to center the ground under the camera.
                ground->SetTransform(Vector3f(-150.0f, 30.0f, -150.0f),
                    Vector3f(0.0f, 0.0f, 0.0f),
                    Vector3f(300.0f, 1.0f, 300.0f));
                context.statusMessage = "Default scene ready.";
            }

            if (mainLight)
                context.selectedNode = mainLight;
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
            if (!context.scene)
                return;

            Vector3f editorPosition(0.0f, 40.0f, 0.0f);
            Vector3f editorRotation(0.0f, 0.0f, 0.0f);
            Vector3f editorScale(1.0f, 1.0f, 1.0f);
            if (context.editorCamera)
                context.editorCamera->GetTransform(editorPosition, editorRotation, editorScale);

            context.scene->Clear();
            context.camera = nullptr;
            context.editorCamera = nullptr;
            context.defaultLight = nullptr;
            context.selectedNode = nullptr;
            context.spawnCounter = 0;
            context.scenePath = "Scenes/untitled.xml";

            Camera* editorCamera = CreateEditorCamera(context);
            if (editorCamera)
                editorCamera->SetTransform(editorPosition, editorRotation, editorScale);

            CreateSceneCamera(context);
            SetupDefaultScene(context);
            context.statusMessage = "New scene created.";
        }
    }
}
