#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>
#include "SingleTon.h"
#include "MessageHandler.h"
#include "RenderWindow.h"

namespace ghost
{
    enum ApplicationType
    {
        APP_WIN32,
        APP_LINUX,
        APP_NUM
    };

	class GHOST_API Application : public MessageHandler
	{
	public:
		Application() = default;
		
		bool initialize(ApplicationType appType, int w, int h, const std::string& wname, bool fullscreen = false);

        virtual ApplicationType getApplicationType() const { return _appType; }

		virtual void tick(float deltaTime) { }
        virtual void onInit() { }
        virtual void onExit() { }
        virtual void onUpdate() { }

		void* getAttachWindow() { return _window->getWindowHandle(); }
        const RenderWindow* getWindow() const { return _window; }

		void run();

		void exitApplication() { _exit = true; }

		void show();

	protected:
		virtual void _messageLoop() { _window->_messageLoop(); }

	protected:
		RenderWindow* _window;

		bool _exit;
		bool _initialize;
        ApplicationType _appType;
	};

    using ApplicationPtr = std::shared_ptr<Application>;
}

#endif
