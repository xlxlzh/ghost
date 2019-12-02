#include "Application.h"

namespace ghost
{
	class ApplicationWin32 : public Application
	{
	public:
		ApplicationWin32() = default;

		virtual bool initialize(int w, int h, const std::string& wname, bool fullscreen /* = false */) override;
	};
}