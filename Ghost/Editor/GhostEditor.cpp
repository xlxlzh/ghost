#include <windows.h>
#include "EditorApplication.h"
#include "Engine.h"

using namespace ghost;

INT CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    ApplicationPtr app = std::make_shared<EditorApplication>();
    app->initialize("GhostEditor", "EditorConfig.xml");

    app->show();
    app->run();
    return 0;
}
