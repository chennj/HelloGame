#include "sopch.h"
#include "Application.h"

#include "stuff\core\Log.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include "stuff\renderer\Renderer.h"

#include "Input.h"

namespace SOMEENGINE
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		Application("../GameEngine/assets/shaders/Texture.glsl");
	}

	Application::Application(const std::string & filePath, const std::string& name)
	{
		SE_PROFILE_FUNCTION();

		SE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		_Window = Window::Create(WindowProps(name));
		_Window->SetEventCallback(SE_BIND_EVENT_FN(Application::OnEvent));
		//设置刷新频率是否垂直同步
		_Window->SetVSync(true);

		Renderer::Init(filePath);

		_ImGuiLayer = new ImGuiLayer();
		PushOverlay(_ImGuiLayer);
	}

	Application::~Application()
	{
		SE_PROFILE_FUNCTION();
	}

	void Application::OnEvent(Event& e)
	{
		SE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e1)->bool
		//{
		//	return this->OnWindowClose(e1);
		//});
		dispatcher.Dispatch<WindowCloseEvent>(SE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SE_BIND_EVENT_FN(Application::OnWindowResize));

		//SE_CORE_TRACE("{0}",e);

		for (auto it = _LayerStack.end(); it != _LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e._Handled)
				break;
		}
	}

	void Application::Close()
	{
		_Running = false;
	}

	void Application::Run()
	{
		SE_PROFILE_FUNCTION();

		while (_Running)
		{
			SE_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - _LastFrameTime;

			_LastFrameTime = time;

			if (!_Minimized)
			{
				{
					SE_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : _LayerStack)
					{
						layer->OnUpdate(timestep);
					}
				}

				_ImGuiLayer->Begin();
				{
					SE_PROFILE_SCOPE("LayStack OnImGuiRender");

					for (Layer* layer : _LayerStack)
					{
						layer->OnImGuiRender();
					}
				}
				_ImGuiLayer->End();
			}


			_Window->OnUpdate();

		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent & e)
	{
		SE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			_Minimized = true;
			return false;
		}

		_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::PushLayer(Layer* layer)
	{
		SE_PROFILE_FUNCTION();

		_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		SE_PROFILE_FUNCTION();

		_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}
