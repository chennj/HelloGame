#include "Application.h"

#include "stuff\events\ApplicationEvent.h"
#include "stuff\Log.h"

namespace SOMEENGINE
{
	Application::Application()
	{
	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		SO_TRACE(e);

		while (true)
		{
			;
		}
	}
}
