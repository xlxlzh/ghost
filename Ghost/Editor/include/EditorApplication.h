#include "Application.h"
#include "ContentBrowser.h"

class EditorApplication : public ghost::Application
{
public:
    EditorApplication() = default;
    virtual ~EditorApplication() = default;
    
    virtual void onTick(float deltaTime) override;
    virtual void onInit() override;
    virtual void onExit() override;
    virtual void onUpdate() override;
    
protected:
    ContentBrowserPagePtr _contentBrowserPage;
};

