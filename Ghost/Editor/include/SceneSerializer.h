#ifndef _SCENE_SERIALIZER_H_
#define _SCENE_SERIALIZER_H_

#include <string>

#include "EditorContext.h"

namespace ghost
{
    namespace editor
    {
        // Save/load the editable scene as XML under <resources>/Scenes/.
        // Paths are relative to the resources root (e.g. "Scenes/level1.xml").
        bool SaveScene(EditorContext& context, const std::string& relativePath);
        bool LoadScene(EditorContext& context, const std::string& relativePath);

        // Refreshes context.sceneFiles from <resources>/Scenes/*.xml.
        void RefreshSceneFiles(EditorContext& context);
    }
}

#endif
