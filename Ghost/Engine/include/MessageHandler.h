#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_

#include "Ghost.h"

namespace ghost
{
	class GHOST_API MessageHandler
	{
	public:
		virtual void onMouseRightClick() { }
		virtual void onMouseLeftClick() { }
		virtual void onKeydown() { }
		virtual void onKeyup() { }
	};
}

#endif
