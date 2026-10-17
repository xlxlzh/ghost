#ifndef _VIEWPORT_INPUT_PANEL_H_
#define _VIEWPORT_INPUT_PANEL_H_

#include "panels/EditorPanel.h"

namespace ghost
{
    namespace editor
    {
        // Invisible panel that drives the free-fly editor camera from ImGui
        // mouse input (RMB look, wheel dolly).
        class ViewportInputPanel final : public EditorPanel
        {
        public:
            explicit ViewportInputPanel(EditorContext& context) : EditorPanel(context) { }
            virtual ~ViewportInputPanel() = default;

            virtual void Show() override;
        };

        DECLAR_SMART_POINTER(ViewportInputPanel)
    }
}

#endif
