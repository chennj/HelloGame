// ���ϵ�ַ //////////////////////////////////////////////
// �����ַ��github.com/TheCherno/hazel
// opengl ��ѧ��
// http://glm.g-truc.net/
// https://github.com/g-truc/glm
// opengl shader compilation
// http://www.khronos.org/opengl/wiki/Shader_Compilation
// -----------------------------------------------------
// ֪ʶ�� ///////////////////////////////////////////////
// -------------
// VertexBuffer��IndexBufferֻ�з���VertexArray�вŻ�������
// -------------
// ���ǹ۲쵽������	= Projection * View * Model * VerticesPosition(Mask)
//					= VP(PV) * Model * VerticesPosition(Mask)
//					= CAMERA * Model * VerticesPosition(Mask)
//					= MVP * VerticesPosition(Mask)
// Projection(Matrix) * View(Matrix) = Camera : ͶӰ�����󣩱任 * ��ͼ�����󣩱任 = �۲��λ��
// Model-Matrix = Object : ģ�ͱ任 = ����λ��
// -------------
// Timestep �����ã�ʹ����֮�������Ƿ�����ǿ�ƴ�ֱͬ����Ҳ����
// ˢ��Ƶ���Ƿ���ͬ�����ͼ�ε��ƶ�Ч������һ�µġ����忴
// ExampleLayer::OnUpdate �� _Window->SetVSync(false/true);
// git commit "TIMESTEP and Delta Time"
// -------------
// OpenGL Version 4.5�汾֮��������һ���µ���չ ARB_direct_state_access,
// �����չΪOpenGL������һ���µ����Ծ���Direct State Acess����ΪDSA���ڰ�
// �����ʱ�򣬺;ɵİ汾д����һ������OpenGLTexture2D�������֡�����DSA����Ϊ����
// ��ƴ����ܶ�ķ��㡣�������Ҫ���������Ƶ�ʱ���object���������ڸ��ֳ�ʼ��ʱ��
// Ҫ��������һ���̶��ϼ�����״̬���л��Ĵ�����
// ----------------------------------------------------
#include "sopch.h"

#include <Some.h>

// -- Entry Point ---------------------------
#include "stuff\core\EntryPoint.h"
// ------------------------------------------

#include "../imgui/imgui.h"

#include "platform\opengl\OpenGLShader.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Sandbox2D.h"


class ExampleLayer : public SOMEENGINE::Layer
{
private:
	// cache shader
	SOMEENGINE::ShaderLibrary _shaderLibrary;

	//������
	SOMEENGINE::Ref<SOMEENGINE::Shader> _Shader;
	SOMEENGINE::Ref<SOMEENGINE::VertexArray> _VertexArray;

	//�ı���
	SOMEENGINE::Ref<SOMEENGINE::Shader> _FlatColorShader;
	SOMEENGINE::Ref<SOMEENGINE::VertexArray> _SquareVA;

	SOMEENGINE::Ref<SOMEENGINE::Texture2D> _Texture, _FlowerTexture;

	SOMEENGINE::OrthographicCameraController _CameraController;

	glm::vec4 _SquareColor = { 0.2, 0.3, 0.8, 1.0 };

public:
	ExampleLayer() 
		:
		Layer("Example"), 
		_CameraController(960.0f / 540.0f, true)
	{
		// ������ //////////////////////////////////////////////////////////////
		_VertexArray = SOMEENGINE::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,
		};

		SOMEENGINE::Ref<SOMEENGINE::VertexBuffer> vertexBuffer;
		vertexBuffer = SOMEENGINE::VertexBuffer::Create(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{ SOMEENGINE::ShaderDataType::Float3, "a_Position" },
			{ SOMEENGINE::ShaderDataType::Float4, "a_Color" }

		});

		_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = {
			0, 1, 2
		};

		SOMEENGINE::Ref<SOMEENGINE::IndexBuffer> indexBuffer;
		indexBuffer = SOMEENGINE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
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
			"uniform mat4 u_Model;\n"
			"\n"
			"out vec3 v_Position;\n"
			"\n"
			"out vec4 v_Color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"   v_Color = a_Color;\n"
			"	v_Position = a_Position;\n"
			"	gl_Position = u_ViewProjection * u_Model * vec4(a_Position,1.0);\n"
			"}\n";

		std::string fragmentSrc =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) out vec4 color;"		// �����GPU�ı���
			"\n"
			"in vec3 v_Position;\n"
			"\n"
			"in vec4 v_Color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	color = v_Color;\n"
			"}\n";

		_Shader = SOMEENGINE::Shader::Create("vertexColorTriangle", vertexSrc, fragmentSrc);

		//�ı��� //////////////////////////////////////////////////////////////
		_SquareVA = SOMEENGINE::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.75f, 0.0f, 0.0, 0.0,
			 0.5f, -0.75f, 0.0f, 1.0, 0.0,
			 0.5f,  0.75f, 0.0f, 1.0, 1.0,
			-0.5f,  0.75f, 0.0f, 0.0, 1.0,
		};
		SOMEENGINE::Ref<SOMEENGINE::VertexBuffer> squareVB;
		squareVB = SOMEENGINE::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		squareVB->SetLayout({
			{ SOMEENGINE::ShaderDataType::Float3, "a_Position" },
			{ SOMEENGINE::ShaderDataType::Float2, "a_TexCoord" }
		});

		_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squreIndices[6] = {
			0, 1, 2,
			2, 3, 0,
		};

		SOMEENGINE::Ref<SOMEENGINE::IndexBuffer> squareIB;
		squareIB = SOMEENGINE::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t));
		_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc2 =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec3 a_Position;"
			"\n"
			"uniform mat4 u_ViewProjection;\n"
			"\n"
			"uniform mat4 u_Model;\n"
			"\n"
			"out vec3 v_Position;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	v_Position = a_Position;\n"
			"	gl_Position = u_ViewProjection * u_Model * vec4(a_Position,1.0);\n"
			"}\n";

		std::string fragmentSrc2 =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) out vec4 color;"		// �����GPU�ı���
			"\n"
			"in vec3 v_Position;\n"
			"\n"
			"uniform vec4 u_Color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	color = u_Color;\n"
			"}\n";

		_FlatColorShader = SOMEENGINE::Shader::Create("vertexColorSquare", vertexSrc2, fragmentSrc2);

		// �y���N�D ////////////////////////////////////////////////////////////////
		
		auto _TextureShader = _shaderLibrary.Load("../GameEngine/assets/shaders/Texture.glsl");

		_Texture		= SOMEENGINE::Texture2D::Create("../res/texture/cnchess/WHITE.GIF");
		_FlowerTexture	= SOMEENGINE::Texture2D::Create("../res/texture/texture-02.png");

		std::dynamic_pointer_cast<SOMEENGINE::OpenGLShader>(_TextureShader)->Bind();
		std::dynamic_pointer_cast<SOMEENGINE::OpenGLShader>(_TextureShader)->UpdateUniformInt("u_Texture", 0);

	}

public:
	void OnUpdate(SOMEENGINE::Timestep ts) override
	{
		_CameraController.OnUpdate(ts);

		SOMEENGINE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		SOMEENGINE::RenderCommand::Clear();

		SOMEENGINE::Renderer::BeginScene(_CameraController.GetCamera());
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<SOMEENGINE::OpenGLShader>(_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<SOMEENGINE::OpenGLShader>(_FlatColorShader)->UpdateUniformFloat4("u_Color", _SquareColor);

		for (int y = 0; y < 20; y++)
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x*0.11f, y*0.16f, 0.0f);
			glm::mat4 modelTransform = glm::translate(glm::mat4(1.0f), pos) * scale;
			SOMEENGINE::Renderer::Submit(_FlatColorShader, _SquareVA, modelTransform);
		}

		auto _TextureShader = _shaderLibrary.Get("Texture");

		_Texture->Bind();
		SOMEENGINE::Renderer::Submit(_TextureShader, _SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		_FlowerTexture->Bind();
		SOMEENGINE::Renderer::Submit(_TextureShader, _SquareVA, 
			glm::translate(glm::mat4(1.0), glm::vec3(0.25,-0.25,0.0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		SOMEENGINE::Renderer::EndScene();
	}

	void OnEvent(SOMEENGINE::Event& event) override
	{
		_CameraController.OnEvent(event);
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color ", &_SquareColor.x);
		ImGui::End();
	}

};

class Sandbox : public SOMEENGINE::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

SOMEENGINE::Application* SOMEENGINE::CreateApplication()
{
	return new Sandbox();
}