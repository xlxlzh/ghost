#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>
#include "SingleTon.h"
#include "MessageHandler.h"
#include "RenderWindow.h"
#include "Timer.h"

namespace ghost
{
    enum PlatformType
    {
        PLATFORM_WIN32,
        PLATFORM_LINUX,
        PLATFORM_NUM
    };

	class GHOST_API Application : public MessageHandler
	{
	public:
		Application() = default;
		
		bool initialize(PlatformType appType, int w, int h, const std::string& wname, bool fullscreen = false);

        virtual PlatformType getPlatformType() const { return _platformType; }

		virtual void onTick(float deltaTime) { }
        virtual void onInit() { }
        virtual void onExit() { }
        virtual void onUpdate() { }

		void* getAttachWindow() { return _window->getWindowHandle(); }
        RenderWindow* getWindow() const { return _window; }

		void run();

		void exitApplication() { _exit = true; }

		void show();
        float getFPS() const;

	protected:
		virtual void _messageLoop() { _window->_messageLoop(); }

        void _updateFrameTime(float ms) { _msPerFrame = ms; }

	protected:
		RenderWindow* _window;

		bool _exit;
		bool _initialize;
        PlatformType _platformType;

        TimerPtr _applicationTimer;

        float _msPerFrame;
	};

    DECLAR_SMART_POINTER(Application)
}

#endif
