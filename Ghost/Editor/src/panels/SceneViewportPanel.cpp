#include "panels/SceneViewportPanel.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace ghost
{
    namespace editor
    {
        void SceneViewportPanel::Show()
        {
            ImGui::Begin("SceneViewport");
            ImGui::End();
        }
    }
}
