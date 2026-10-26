#ifndef _DOCKSPACE_PANEL_H_
#define _DOCKSPACE_PANEL_H_

#include "EditorPanel.h"

namespace ghost
{
    namespace editor
    {
        class DockspacePanel : public EditorPanel
        {
        public:
            explicit DockspacePanel(EditorContext& context) : EditorPanel(context) { }
            virtual ~DockspacePanel() = default;

            virtual void Show() override;
            
        private:
            bool _layoutInitialized = false;
        };
        
        DECLAR_SMART_POINTER(DockspacePanel);
    }
}

#endif