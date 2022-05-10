#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_

#include "Ghost.h"

namespace ghost
{
    enum class Key
    {

    };

	class GHOST_API MessageHandler
	{
	public:
		virtual void onMouseRightClick() { }
		virtual void onMouseLeftClick() { }
		virtual void onKeydown(Key key) { }
		virtual void onKeyup(Key key) { }
	};
}

#endif
