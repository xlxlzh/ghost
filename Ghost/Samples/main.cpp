#include <windows.h>
#include "Application.h"
#include "LogManager.h"
#include "Engine.h"

using namespace ghost;

INT WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
    ApplicationPtr app = std::make_shared<Application>();
    app->initialize(APP_WIN32, 800, 600, "Ghost");

    Engine::getInstance()->initEngine(RENDER_D3D11, app, _4x);

	app->show();
	app->run();
	return 0;
}