#include "SceneSerializer.h"

#include <algorithm>
#include <filesystem>

#include "Engine.h"
#include "FileSystem.h"
#include "Scene.h"
#include "Camera.h"
#include "EditorScene.h"

namespace ghost
{
    namespace editor
    {
        namespace
        {
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

            SceneNode* FindFirstSelectable(SceneNode* node)
            {
                if (!node)
                    return nullptr;

                for (SceneNode* child : node->GetChildren())
                {
                    if (!child || child->IsEditorOnly())
                        continue;

                    const SceneNodeType type = child->GetType();
                    if (type == SCENENODE_LIGHT || type == SCENENODE_MODEL)
                        return child;

                    if (SceneNode* found = FindFirstSelectable(child))
                        return found;
                }

                return nullptr;
            }
        }

        bool SaveScene(EditorContext& context, const std::string& relativePath)
        {
            if (!context.sceneManager || !context.sceneData)
            {
                context.statusMessage = "Cannot save scene: no scene data.";
                return false;
            }

            const std::string normalizedPath = NormalizeScenePath(relativePath);
            const std::string fullPath = AddTrailingSlash(context.resourcesPath) + normalizedPath;
            EnsureDirectory(fullPath);

            context.sceneManager->SyncAllToData();

            FileStream stream(fullPath, AM_WRITE);
            if (!stream.IsOpened())
            {
                context.statusMessage = "Failed to open scene for writing: " + normalizedPath;
                return false;
            }

            const bool saved = context.sceneData->Save(stream);
            stream.Close();

            if (!saved)
            {
                context.statusMessage = "Failed to save scene: " + normalizedPath;
                return false;
            }

            context.sceneData->SetName(normalizedPath);
            context.scenePath = normalizedPath;
            context.statusMessage = "Saved scene: " + normalizedPath;
            return true;
        }

        bool LoadScene(EditorContext& context, const std::string& relativePath)
        {
            if (!context.sceneManager)
            {
                context.statusMessage = "Cannot load scene: no scene manager.";
                return false;
            }

            const std::string normalizedPath = NormalizeScenePath(relativePath);
            const std::string fullPath = AddTrailingSlash(context.resourcesPath) + normalizedPath;

            ScenePtr scene = std::make_shared<Scene>();
            scene->SetName(normalizedPath);

            FileStream stream(fullPath, AM_READ);
            if (!stream.IsOpened() || !scene->Load(stream))
            {
                stream.Close();
                context.statusMessage = "Failed to load scene: " + normalizedPath;
                return false;
            }
            stream.Close();

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

            context.sceneManager->SetScene(scene);
            context.sceneData = scene;

            Camera* editorCamera = CreateEditorCamera(context);
            if (editorCamera)
                editorCamera->SetTransform(editorPosition, editorRotation, editorScale);

            Camera* sceneCamera = nullptr;
            for (SceneNode* node : context.sceneManager->GetSceneNodes())
            {
                if (node && !node->IsEditorOnly() && node->GetType() == SCENENODE_CAMERA)
                {
                    sceneCamera = static_cast<Camera*>(node);
                    break;
                }
            }

            if (!sceneCamera)
                sceneCamera = CreateSceneCamera(context);

            context.camera = sceneCamera;

            if (sceneCamera)
            {
                const float aspect = context.viewportAspect > 0.0f ? context.viewportAspect : 1.0f;
                sceneCamera->SetProjectParams(sceneCamera->GetFov(), aspect,
                    sceneCamera->GetNear(), sceneCamera->GetFar());
                context.sceneManager->SyncNodeToData(sceneCamera);
            }

            context.defaultLight = context.sceneManager->GetMainLigt();
            context.selectedNode = FindFirstSelectable(context.sceneManager->GetRootNode());
            context.scenePath = normalizedPath;
            context.statusMessage = "Loaded scene: " + normalizedPath;

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
