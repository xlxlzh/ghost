#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_

namespace ghost
{
	class MessageHandler
	{
	public:
		virtual void onMouseRightClick() { }
		virtual void onMouseLeftClick() { }
		virtual void onKeydown() { }
		virtual void onKeyup() { }
	};
}

#endif
