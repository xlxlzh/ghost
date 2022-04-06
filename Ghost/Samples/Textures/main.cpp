#include <windows.h>
#include "TexturesApplication.h"

INT CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    ApplicationPtr app = std::make_shared<TexturesApplication>();
    app->initialize("TexturesApplication", "GhostConfig.xml");

    app->show();
    app->run();
    return 0;
}