#ifndef _HIERARCHY_PANEL_H_
#define _HIERARCHY_PANEL_H_

#include "panels/EditorPanel.h"

namespace ghost
{
    namespace editor
    {
        class HierarchyPanel final : public EditorPanel
        {
        public:
            explicit HierarchyPanel(EditorContext& context) : EditorPanel(context) { }
            virtual ~HierarchyPanel() = default;

            virtual void Show() override;

        private:
            void DrawNode(SceneNode* node);
        };

        DECLAR_SMART_POINTER(HierarchyPanel)
    }
}

#endif
