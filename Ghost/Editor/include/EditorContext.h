#ifndef _EDITOR_CONTEXT_H_
#define _EDITOR_CONTEXT_H_

#include <string>
#include <vector>

#include "Ghost.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Material.h"

namespace ghost
{
    namespace editor
    {
        // Shared state for all editor panels. Panels should only communicate
        // through this context so new panels can be added without touching
        // the existing ones.
        class EditorContext
        {
        public:
            SceneManager* scene = nullptr;
            Camera* camera = nullptr;        // scene camera (visible, serialized)
            Camera* editorCamera = nullptr;  // free-fly viewport camera (editor-only)
            Light* defaultLight = nullptr;
            SceneNode* selectedNode = nullptr;

            // The scene camera is used for rendering only while it is selected;
            // otherwise the viewport uses the editor camera.
            Camera* GetRenderCamera() const
            {
                if (camera && selectedNode == camera)
                    return camera;

                return editorCamera ? editorCamera : camera;
            }

            // While looking through the scene camera, keep the editor camera in
            // the same pose. Then leaving the scene camera (selection change or
            // mouse navigation) does not snap the view to an old position.
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
            MaterialPtr defaultMaterial;
            std::vector<std::string> modelAssets; // logical paths, e.g. "Meshes/cube.obj"
            std::vector<std::string> sceneFiles;  // logical paths, e.g. "Scenes/forest.xml"
            std::string scenePath = "Scenes/editor_scene.xml";
            float viewportAspect = 1.0f;
            unsigned spawnCounter = 0;
            std::string statusMessage;
        };
    }
}

#endif
