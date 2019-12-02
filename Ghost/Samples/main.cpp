#include "RenderWindowWin32.h"
#include "ApplicationWin32.h"

using namespace ghost;

INT WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	Application* app = new ApplicationWin32();
	app->initialize(800, 600, "Ghost");
	app->show();
	app->run();
	return 0;
}