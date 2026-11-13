#ifndef _MENU_BAR_PANEL_H_
#define _MENU_BAR_PANEL_H_

#include "panels/EditorPanel.h"

namespace ghost
{
    namespace editor
    {
        class MenuBarPanel final : public EditorPanel
        {
        public:
            explicit MenuBarPanel(EditorContext& context) : EditorPanel(context) { }
            virtual ~MenuBarPanel() = default;

            virtual void Show() override;

        private:
            char _dialogPath[256] = {};
        };

        DECLAR_SMART_POINTER(MenuBarPanel)
    }
}

#endif
