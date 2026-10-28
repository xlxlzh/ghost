#include "panels/SceneViewportPanel.h"

#include <cstdint>

#include "imgui.h"

namespace ghost
{
    namespace editor
    {
        void SceneViewportPanel::Show()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            const bool visible = ImGui::Begin("SceneViewport", nullptr,
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::PopStyleVar();

            if (!visible)
            {
                _context.viewportHovered = false;
                _context.viewportFocused = false;
                ImGui::End();
                return;
            }

            const ImVec2 available = ImGui::GetContentRegionAvail();
            const ImVec2 framebufferScale = ImGui::GetIO().DisplayFramebufferScale;
            const float scaleX = framebufferScale.x > 0.0f ? framebufferScale.x : 1.0f;
            const float scaleY = framebufferScale.y > 0.0f ? framebufferScale.y : 1.0f;

            unsigned width = 0;
            unsigned height = 0;

            if (available.x > 0.0f && available.y > 0.0f)
            {
                width = static_cast<unsigned>(available.x * scaleX);
                height = static_cast<unsigned>(available.y * scaleY);

                if (width == 0)
                    width = 1;
                if (height == 0)
                    height = 1;
            }

            _context.viewportWidth = width;
            _context.viewportHeight = height;
            _context.viewportHovered = ImGui::IsWindowHovered();
            _context.viewportFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);

            if (width == 0 || height == 0)
            {
                ImGui::End();
                return;
            }

            bool displayed = false;

            if (_context.viewportRenderTarget)
            {
                auto textureView = _context.viewportRenderTarget->GetColorTextureView(0);

                if (textureView)
                {
                    const uint64_t textureID = textureView->GetNativeHandle();
                    if (textureID != 0)
                    {
                        ImGui::Image((ImTextureID)textureID,
                            available, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
                        displayed = true;
                    }
                }
            }

            if (!displayed)
                ImGui::TextDisabled("Viewport initializing...");

            ImGui::End();
        }
    }
}
