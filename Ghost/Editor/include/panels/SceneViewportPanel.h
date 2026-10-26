#ifndef _SCENEVIEWPORT_PANEL_H_
#define _SCENEVIEWPORT_PANEL_H_

#include "EditorPanel.h"

namespace ghost
{
    namespace editor
    {
        class SceneViewportPanel : public EditorPanel
        {
        public:
            explicit SceneViewportPanel(EditorContext& context) : EditorPanel(context) { }
            virtual ~SceneViewportPanel() = default;

            virtual void Show() override;
            
        private:
            float _lastAspect = 0.0f;
        };
        
        DECLAR_SMART_POINTER(SceneViewportPanel);
    }
}

#endif