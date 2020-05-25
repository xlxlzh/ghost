#include <windows.h>
#include "SampleApplication.h"
#include "LogManager.h"
#include "Engine.h"

using namespace ghost;

INT WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
    ApplicationPtr app = std::make_shared<SampleApplication>();
    app->initialize(APP_WIN32, 980, 540, "Ghost");

	app->show();
	app->run();
	return 0;
}