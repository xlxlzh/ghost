#include <windows.h>
#include "Application.h"
#include "LogManager.h"

using namespace ghost;

INT WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
    LogManager::getInstance()->addLog("debug.txt");
    LogManager::getInstance()->logMessage(LOG_DEBUG, "debug info");
    LogManager::getInstance()->logMessage(LOG_ERROR, "error info");
    LogManager::getInstance()->logMessage(LOG_INFO, "info info");
    LogManager::getInstance()->logMessage(LOG_WARNING, "warning info");

    Application* app = new Application();
	app->initialize(APP_WIN32 ,800, 600, "Ghost");
	app->show();
	app->run();
	return 0;
}