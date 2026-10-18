#ifndef _EDITOR_PANEL_H_
#define _EDITOR_PANEL_H_

#include "GUIManager.h"
#include "EditorContext.h"

namespace ghost
{
    namespace editor
    {
        class EditorPanel : public GuiPage
        {
        public:
            explicit EditorPanel(EditorContext& context) : _context(context) { }
            virtual ~EditorPanel() = default;

        protected:
            EditorContext& _context;
        };
    }
}

#endif
