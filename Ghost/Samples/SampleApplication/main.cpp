#include <windows.h>
#include "SampleApplication.h"
#include "LogManager.h"
#include "Engine.h"

using namespace ghost;

INT CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
    ApplicationPtr app = std::make_shared<SampleApplication>();
    app->Initialize("SampleApplication", "GhostConfig.xml");

	app->Show();
	app->Run();
	return 0;
}