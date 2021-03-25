#include "sopch.h"
#include "Application.h"

#include "stuff\events\ApplicationEvent.h"
#include "stuff\Log.h"

namespace SOMEENGINE
{
	Application::Application()
	{
		_Window = std::unique_ptr<Window>(Window::Create());
	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (_Running)
		{
			_Window->OnUpdate();
		}
	}
}
