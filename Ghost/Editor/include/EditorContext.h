#ifndef _EDITOR_CONTEXT_H_
#define _EDITOR_CONTEXT_H_

#include <string>
#include <vector>

#include "Ghost.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"

namespace ghost
{
    namespace editor
    {
        class EditorContext
        {
        public:
            SceneManager* sceneManager = nullptr;
            ScenePtr sceneData;
            Camera* camera = nullptr;
            Camera* editorCamera = nullptr;
            Light* defaultLight = nullptr;
            SceneNode* selectedNode = nullptr;

            Camera* GetRenderCamera() const
            {
                if (camera && selectedNode == camera)
                    return camera;

                return editorCamera ? editorCamera : camera;
            }

            void SyncEditorCameraToSceneCamera()
            {
                if (!camera || !editorCamera || selectedNode != camera)
                    return;

                Vector3f position;
                Vector3f rotation;
                Vector3f scale;
                camera->GetTransform(position, rotation, scale);
                editorCamera->SetTransform(position, rotation, scale);
            }

            std::string resourcesPath;
            std::vector<std::string> modelAssets;
            std::vector<std::string> sourceMeshAssets;
            std::vector<std::string> sceneFiles;
            std::string scenePath = "Scenes/editor_scene.gmap";
            float viewportAspect = 1.0f;
            unsigned spawnCounter = 0;
            std::string statusMessage;
        };
    }
}

#endif
