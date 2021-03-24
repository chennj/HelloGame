#include "sopch.h"
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
		SE_TRACE(e);

		while (true)
		{
			;
		}
	}
}
