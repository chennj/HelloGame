#include "sopch.h"
#include "Application.h"

#include "stuff\Log.h"
#include "GLFW\glfw3.h"

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

		for (auto it = _LayerStack.end(); it != _LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e._Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (_Running)
		{
			glClearColor(0.25f, 0.25f, 0.25f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : _LayerStack)
				layer->OnUpdate();

			_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		_LayerStack.PushOverlay(overlay);
	}
}
