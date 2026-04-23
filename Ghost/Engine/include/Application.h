#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>
#include "SingleTon.h"
#include "MessageHandler.h"
#include "RenderWindow.h"
#include "Timer.h"
#include "GhostConfig.h"

namespace ghost
{
	class GHOST_API Application : public MessageHandler
	{
	public:
		Application() = default;
		
		bool Initialize(const std::string& wname, const std::string& configFile);

        virtual PlatformType GetPlatformType() const { return _platformType; }

		virtual void OnTick(float deltaTime) { }
        virtual void OnInit() { }
        virtual void OnExit() { }
        virtual void OnUpdate() { }

		void* GetAttachWindow() { return _window->GetWindowHandle(); }
        RenderWindow* GetWindow() const { return _window; }

		void Run();

		void ExitApplication() { _exit = true; }

		void Show();
        float GetFPS() const;

	protected:
		virtual void InternalMessageLoop() { _window->InternalMessageLoop(); }

        void UpdateFrameTime(float ms) { _msPerFrame = ms; }

	protected:
		RenderWindow* _window;

        bool _exit{ false };
        bool _initialize{ false };
        PlatformType _platformType;

        TimerPtr _applicationTimer;

        float _msPerFrame;
	};

    DECLAR_SMART_POINTER(Application)
}

#endif
