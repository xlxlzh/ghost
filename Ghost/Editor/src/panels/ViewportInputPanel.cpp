#include "panels/ViewportInputPanel.h"

#include <algorithm>
#include <cmath>

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
            const bool rightMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);

            if (!rightMouseDown)
                _looking = false;

            if (_context.selectedNode == _context.camera && !io.WantCaptureMouse &&
                (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || io.MouseWheel != 0.0f))
            {
                _context.selectedNode = nullptr;
            }

            if (_context.GetRenderCamera() != _context.editorCamera)
                return;

            if (io.WantCaptureKeyboard)
                return;

            if (!_looking && rightMouseDown && !io.WantCaptureMouse)
                _looking = true;

            Vector3f position;
            Vector3f rotation;
            Vector3f scale;
            _context.editorCamera->GetTransform(position, rotation, scale);

            bool changed = false;

            if (_looking)
            {
                if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f)
                {
                    rotation._y += io.MouseDelta.x * 0.15f;
                    rotation._x += io.MouseDelta.y * 0.15f;
                    rotation._x = std::max(-89.0f, std::min(89.0f, rotation._x));
                    changed = true;
                }

                if (io.MouseWheel != 0.0f)
                {
                    _moveSpeed *= std::pow(1.2f, io.MouseWheel);
                    _moveSpeed = std::max(0.1f, std::min(1000.0f, _moveSpeed));
                }

                const Matrix4x4f& cameraAbs = _context.editorCamera->GetAbsTrans();
                Vector3f forward(cameraAbs._31, cameraAbs._32, cameraAbs._33);
                Vector3f right(cameraAbs._11, cameraAbs._12, cameraAbs._13);

                if (forward.Length() > 0.0001f)
                    forward.Normalize();
                if (right.Length() > 0.0001f)
                    right.Normalize();

                Vector3f move(0.0f, 0.0f, 0.0f);

                if (ImGui::IsKeyDown(ImGuiKey_W))
                    move += forward;
                if (ImGui::IsKeyDown(ImGuiKey_S))
                    move -= forward;
                if (ImGui::IsKeyDown(ImGuiKey_D))
                    move += right;
                if (ImGui::IsKeyDown(ImGuiKey_A))
                    move -= right;
                if (ImGui::IsKeyDown(ImGuiKey_E))
                    move._y += 1.0f;
                if (ImGui::IsKeyDown(ImGuiKey_Q))
                    move._y -= 1.0f;

                const float length = move.Length();
                if (length > 0.0001f)
                {
                    move /= length;

                    float speed = _moveSpeed;
                    if (io.KeyShift)
                        speed *= 4.0f;
                    if (io.KeyCtrl)
                        speed *= 0.25f;

                    const float deltaTime = std::min(io.DeltaTime, 0.1f);
                    position += move * (speed * deltaTime);
                    changed = true;
                }
            }
            else if (io.MouseWheel != 0.0f && !io.WantCaptureMouse)
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
