#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>
#include "SingleTon.h"
#include "MessageHandler.h"
#include "RenderWindow.h"

namespace ghost
{
	class Application : public MessageHandler
	{
	public:
		Application() = default;
		
		virtual bool initialize(int w, int h, const std::string& wname, bool fullscreen = false) = 0;

		virtual void tick(float deltaTime) { }

		void run();

		void exitApplication() { _exit = true; }

		void show();

	protected:
		virtual void _messageLoop() { _window->_messageLoop(); }

	protected:
		RenderWindow* _window;

		bool _exit;
		bool _initialize;
	};
}

#endif
