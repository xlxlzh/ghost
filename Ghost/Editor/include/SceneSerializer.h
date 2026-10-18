#ifndef _SCENE_SERIALIZER_H_
#define _SCENE_SERIALIZER_H_

#include <string>

#include "EditorContext.h"

namespace ghost
{
    namespace editor
    {
        bool SaveScene(EditorContext& context, const std::string& relativePath);
        bool LoadScene(EditorContext& context, const std::string& relativePath);

        void RefreshSceneFiles(EditorContext& context);
    }
}

#endif
