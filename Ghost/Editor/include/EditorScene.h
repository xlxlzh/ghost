#ifndef _EDITOR_SCENE_H_
#define _EDITOR_SCENE_H_

#include <string>

#include "EditorContext.h"
#include "ModelNode.h"

namespace ghost
{
    namespace editor
    {
        Camera* CreateSceneCamera(EditorContext& context);
        Camera* CreateEditorCamera(EditorContext& context);
        void SetupDefaultScene(EditorContext& context);
        void NewScene(EditorContext& context);
        void RefreshModelAssets(EditorContext& context);

        // Selects node and moves the editor camera to frame it. If node is the
        // scene camera, only selection changes (the viewport looks through it).
        void FocusEditorCamera(EditorContext& context, SceneNode* node);

        ModelNode* SpawnModel(EditorContext& context, const std::string& logicalModelPath);
        Light* SpawnLight(EditorContext& context, LightType lightType);

        // Removes and deletes a node. The editor camera cannot be deleted.
        bool DeleteNode(EditorContext& context, SceneNode* node);

        std::string MakeUniqueNodeName(const EditorContext& context, const std::string& baseName);
    }
}

#endif
