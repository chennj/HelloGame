#include "sopch.h"
#include "Application.h"

#include "stuff\Log.h"

namespace SOMEENGINE
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		_Window = std::unique_ptr<Window>(Window::Create());
		_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}


	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e1)->bool
		//{
		//	return this->OnWindowClose(e1);
		//});
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		SE_CORE_TRACE("{0}",e);
	}

	void Application::Run()
	{
		while (_Running)
		{
			_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_Running = false;
		return true;
	}

}
