#ifndef _INSPECTOR_PANEL_H_
#define _INSPECTOR_PANEL_H_

#include "panels/EditorPanel.h"

namespace ghost
{
    namespace editor
    {
        class InspectorPanel final : public EditorPanel
        {
        public:
            explicit InspectorPanel(EditorContext& context) : EditorPanel(context) { }
            virtual ~InspectorPanel() = default;

            virtual void Show() override;
        };

        DECLAR_SMART_POINTER(InspectorPanel)
    }
}

#endif
