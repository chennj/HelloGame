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

		// 三角形 //////////////////////////////////////////////////////////////
		_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,
		};
		
		_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }

		};

		_VertexBuffer->SetLayout(layout);
		_VertexArray->AddVertexBuffer(_VertexBuffer);

		uint32_t indices[3] = {
			0, 1, 2
		};

		_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		_VertexArray->SetIndexBuffer(_IndexBuffer);

		std::string vertexSrc =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec3 a_Position;"	// 输入上面定义的vertices数组的地址，是一个三维向量
														// 'a_position' is related to 'float vertices[3 * 3]' above
														// 'location=0' is related to '0' of 'glVertexAttribPointer(0,..)' above
			"\n"
			"layout(location = 1) in vec4 a_Color;"
			"\n"
			"out vec3 v_Position;\n"
			"\n"
			"out vec4 v_Color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"   v_Color = a_Color;\n"
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
			"in vec4 v_Color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	color = v_Color;\n"
			"}\n";

		_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		//四边形 //////////////////////////////////////////////////////////////
		_SquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
		};
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }

		});

		_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squreIndices[6] = {
			0, 1, 2,
			2, 3, 0,
		};

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc2 =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec3 a_Position;"	
			"\n"
			"out vec3 v_Position;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	v_Position = a_Position;\n"
			"	gl_Position = vec4(a_Position,1.0);\n"
			"}\n";

		std::string fragmentSrc2 =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) out vec4 color;"		// 输出给GPU的变量
			"\n"
			"in vec3 v_Position;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	color = vec4(0.2, 0.3, 0.8, 1.0);\n"
			"}\n";

		_ShaderSquare.reset(new Shader(vertexSrc2, fragmentSrc2));
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

			// 先画矩形
			_ShaderSquare->Bind();
			_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, _SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			_Shader->Bind();
			_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, _IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
