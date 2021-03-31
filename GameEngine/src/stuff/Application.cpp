#include "sopch.h"
#include "Application.h"

#include "stuff\Log.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include "Input.h"

namespace SOMEENGINE
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		_Window = std::unique_ptr<Window>(Window::Create());
		_Window->SetEventCallback(SE_BIND_EVENT_FN(Application::OnEvent));

		_ImGuiLayer = new ImGuiLayer();
		PushOverlay(_ImGuiLayer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
		};

		glGenVertexArrays(1, &_VertexArray);
		glBindVertexArray(_VertexArray);

		glGenBuffers(1, &_VertextBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _VertextBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

		unsigned int indices[3] = {
			0, 1, 2
		};

		glGenBuffers(1, &_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec3 a_Position;"	// 输入上面定义的vertices数组的地址，是一个三维向量
														// 'a_position' is related to 'float vertices[3 * 3]' above
														// 'location=0' is related to '0' of 'glVertexAttribPointer(0,..)' above
			"\n"
			"out vec3 v_Position;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	v_Position = a_Position;\n"
			"	gl_Position = vec4(a_Position,1.0);\n"
			"}\n";
		
		std::string fragmentSrc =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) out vec4 color;"		// 输出给GPU的变量
			"\n"
			"in vec3 v_Position;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	color = vec4(v_Position, 1.0);\n"
			"}\n";

		_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
		dispatcher.Dispatch<WindowCloseEvent>(SE_BIND_EVENT_FN(Application::OnWindowClose));

		//SE_CORE_TRACE("{0}",e);

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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			_Shader->Bind();
			glBindVertexArray(_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : _LayerStack)
			{
				layer->OnUpdate();
			}

			_ImGuiLayer->Begin();
			for (Layer* layer : _LayerStack)
			{
				layer->OnImGuiRender();
			}
			_ImGuiLayer->End();

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
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}
