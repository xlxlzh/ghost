#include "GUIManager.h"
#include "Color.h"
#include "Vector3.h"

class ContentBrowserPage : public ghost::GuiPage
{
public:
    ContentBrowserPage();

    virtual void Show();
};

DECLAR_SMART_POINTER(ContentBrowserPage);