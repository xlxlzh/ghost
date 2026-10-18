#include "panels/ViewportInputPanel.h"

#include <algorithm>

#include "imgui.h"
#include "Camera.h"

namespace ghost
{
    namespace editor
    {
        void ViewportInputPanel::Show()
        {
            if (!_context.editorCamera)
                return;

            ImGuiIO& io = ImGui::GetIO();

            if (io.WantCaptureMouse)
                return;

            if (_context.selectedNode == _context.camera &&
                (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || io.MouseWheel != 0.0f))
            {
                _context.selectedNode = nullptr;
            }

            if (_context.GetRenderCamera() != _context.editorCamera)
                return;

            Vector3f position;
            Vector3f rotation;
            Vector3f scale;
            _context.editorCamera->GetTransform(position, rotation, scale);

            bool changed = false;

            if (ImGui::IsMouseDown(ImGuiMouseButton_Right) &&
                (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
            {
                rotation._y += io.MouseDelta.x * 0.15f;
                rotation._x += io.MouseDelta.y * 0.15f;
                rotation._x = std::max(-89.0f, std::min(89.0f, rotation._x));
                changed = true;
            }

            if (io.MouseWheel != 0.0f)
            {
                const Matrix4x4f& cameraAbs = _context.editorCamera->GetAbsTrans();
                Vector3f forward(cameraAbs._31, cameraAbs._32, cameraAbs._33);

                const float length = forward.Length();
                if (length > 0.0001f)
                {
                    forward /= length;
                    position += forward * (io.MouseWheel * 2.0f);
                    changed = true;
                }
            }

            if (changed)
                _context.editorCamera->SetTransform(position, rotation, scale);
        }
    }
}
