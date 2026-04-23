#include "Application.h"
#include "ContentBrowser.h"

class EditorApplication : public ghost::Application
{
public:
    EditorApplication() = default;
    virtual ~EditorApplication() = default;
    
    virtual void OnTick(float deltaTime) override;
    virtual void OnInit() override;
    virtual void OnExit() override;
    virtual void OnUpdate() override;
    
protected:
    ContentBrowserPagePtr _contentBrowserPage;
};

