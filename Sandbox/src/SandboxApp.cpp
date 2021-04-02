// 资料地址 //////////////////////////////////////////////
// opengl 数学库
// http://glm.g-truc.net/
// https://github.com/g-truc/glm
// opengl shader compilation
// http://www.khronos.org/opengl/wiki/Shader_Compilation
// -----------------------------------------------------
// 知识点 ///////////////////////////////////////////////
// -------------
// VertexBuffer、IndexBuffer只有放入VertexArray中才会起作用
// -------------
// 我们观察到的物体	= Projection * View * Model * VerticesPosition(Mask)
//					= VP(PV) * Model * VerticesPosition(Mask)
//					= CAMERA * Model * VerticesPosition(Mask)
//					= MVP * VerticesPosition(Mask)
// Projection(Matrix) * View(Matrix) = Camera : 投影（矩阵）变换 * 视图（矩阵）变换 = 观察点位置
// Model-Matrix = Object : 模型变换 = 物体位置
// -------------
// Timestep 的作用：使用它之后，无论是否设置强制垂直同步，也不管
// 刷新频率是否相同，最后图形的移动效果都是一致的。具体看
// ExampleLayer::OnUpdate 和 _Window->SetVSync(false/true);
// ----------------------------------------------------
#include "sopch.h"

#include <Some.h>

#include "../imgui/imgui.h"

class ExampleLayer : public SOMEENGINE::Layer
{
private:
	std::shared_ptr<SOMEENGINE::Shader> _Shader;
	std::shared_ptr<SOMEENGINE::VertexArray> _VertexArray;

	//四边形
	std::shared_ptr<SOMEENGINE::Shader> _ShaderSquare;
	std::shared_ptr<SOMEENGINE::VertexArray> _SquareVA;

	SOMEENGINE::OrthographicCamera _Camera;

	glm::vec3 _CameraPosition;
	float _CameraMoveSpeed = 1.0f;
	float _CameraRotation = 0.0f;
	float _CameraRotationSpeed = 30.0f;

public:
	ExampleLayer() :Layer("Example"), _Camera(-2.0f, 2.0f, -1.0f, 1.0f), _CameraPosition(0.0)
	{
		// 三角形 //////////////////////////////////////////////////////////////
		_VertexArray.reset(SOMEENGINE::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<SOMEENGINE::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(SOMEENGINE::VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{ SOMEENGINE::ShaderDataType::Float3, "a_Position" },
			{ SOMEENGINE::ShaderDataType::Float4, "a_Color" }

		});

		_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = {
			0, 1, 2
		};

		std::shared_ptr<SOMEENGINE::IndexBuffer> indexBuffer;
		indexBuffer.reset(SOMEENGINE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec3 a_Position;"
			"\n"
			"layout(location = 1) in vec4 a_Color;"
			"\n"
			"uniform mat4 u_ViewProjection;\n"
			"\n"
			"out vec3 v_Position;\n"
			"\n"
			"out vec4 v_Color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"   v_Color = a_Color;\n"
			"	v_Position = a_Position;\n"
			"	gl_Position = u_ViewProjection * vec4(a_Position,1.0);\n"
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

		_Shader.reset(new SOMEENGINE::Shader(vertexSrc, fragmentSrc));

		//四边形 //////////////////////////////////////////////////////////////
		_SquareVA.reset(SOMEENGINE::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
		};
		std::shared_ptr<SOMEENGINE::VertexBuffer> squareVB;
		squareVB.reset(SOMEENGINE::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ SOMEENGINE::ShaderDataType::Float3, "a_Position" }

		});

		_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squreIndices[6] = {
			0, 1, 2,
			2, 3, 0,
		};

		std::shared_ptr<SOMEENGINE::IndexBuffer> squareIB;
		squareIB.reset(SOMEENGINE::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc2 =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec3 a_Position;"
			"\n"
			"uniform mat4 u_ViewProjection;\n"
			"\n"
			"out vec3 v_Position;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	v_Position = a_Position;\n"
			"	gl_Position = u_ViewProjection * vec4(a_Position,1.0);\n"
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

		_ShaderSquare.reset(new SOMEENGINE::Shader(vertexSrc2, fragmentSrc2));
	}

public:
	void OnUpdate(SOMEENGINE::Timestep ts) override
	{
		//SE_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if (SOMEENGINE::Input::IsKeyPressed(SE_KEY_LEFT))
			_CameraPosition.x -= _CameraMoveSpeed * ts;
		else if (SOMEENGINE::Input::IsKeyPressed(SE_KEY_RIGHT))
			_CameraPosition.x += _CameraMoveSpeed * ts;

		if (SOMEENGINE::Input::IsKeyPressed(SE_KEY_UP))
			_CameraPosition.y += _CameraMoveSpeed * ts;
		else if (SOMEENGINE::Input::IsKeyPressed(SE_KEY_DOWN))
			_CameraPosition.y -= _CameraMoveSpeed * ts;

		if (SOMEENGINE::Input::IsKeyPressed(SE_KEY_A))
			_CameraRotation -= _CameraRotationSpeed * ts;
		if (SOMEENGINE::Input::IsKeyPressed(SE_KEY_D))
			_CameraRotation += _CameraRotationSpeed * ts;

		SOMEENGINE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		SOMEENGINE::RenderCommand::Clear();

		// Camera向右上角移动，相当于物体向左下角移动
		_Camera.SetPosition(_CameraPosition);
		// Camera绕Z轴逆时针旋转n度，相当于物体绕Z轴顺时针旋转n度
		_Camera.SetRotation(_CameraRotation);

		SOMEENGINE::Renderer::BeginScene(_Camera);

		SOMEENGINE::Renderer::Submit(_ShaderSquare, _SquareVA);
		SOMEENGINE::Renderer::Submit(_Shader, _VertexArray);

		SOMEENGINE::Renderer::EndScene();
	}

	void OnEvent(SOMEENGINE::Event& event) override
	{
		SOMEENGINE::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<SOMEENGINE::KeyPressedEvent>(SE_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	void OnImGuiRender()
	{
	}

	bool OnKeyPressedEvent(SOMEENGINE::KeyPressedEvent& event)
	{
		//if (event.GetKeyCode() == SE_KEY_LEFT)
		//	_CameraPosition.x -= _CameraSpeed;
		//if (event.GetKeyCode() == SE_KEY_RIGHT)
		//	_CameraPosition.x += _CameraSpeed;

		//if (event.GetKeyCode() == SE_KEY_DOWN)
		//	_CameraPosition.y -= _CameraSpeed;
		//if (event.GetKeyCode() == SE_KEY_UP)
		//	_CameraPosition.y += _CameraSpeed;

		return false;
	}
};

class Sandbox : public SOMEENGINE::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

SOMEENGINE::Application* SOMEENGINE::CreateApplication()
{
	return new Sandbox();
}