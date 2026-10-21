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
        bool ImportModelAsset(EditorContext& context, const std::string& sourceLogicalPath,
            const std::string& destLogicalPath);
        bool ExportModelAssetXml(EditorContext& context, const std::string& logicalModelPath);

        void FocusEditorCamera(EditorContext& context, SceneNode* node);

        ModelNode* SpawnModel(EditorContext& context, const std::string& logicalModelPath);
        Light* SpawnLight(EditorContext& context, LightType lightType);

        bool DeleteNode(EditorContext& context, SceneNode* node);

        std::string MakeUniqueNodeName(const EditorContext& context, const std::string& baseName);
    }
}

#endif
