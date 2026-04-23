#include <windows.h>
#include "PBRLighting.h"

INT CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    ApplicationPtr app = std::make_shared<PBRLightingApplication>();
    app->Initialize("PBRLightingApplication", "GhostConfig.xml");

    app->Show();
    app->Run();
    return 0;
}