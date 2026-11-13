#ifndef _CONTENT_BROWSER_PANEL_H_
#define _CONTENT_BROWSER_PANEL_H_

#include "panels/EditorPanel.h"

namespace ghost
{
    namespace editor
    {
        class ContentBrowserPanel final : public EditorPanel
        {
        public:
            explicit ContentBrowserPanel(EditorContext& context) : EditorPanel(context) { }
            virtual ~ContentBrowserPanel() = default;

            virtual void Show() override;
        };

        DECLAR_SMART_POINTER(ContentBrowserPanel)
    }
}

#endif
