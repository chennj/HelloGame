#include "Sandbox2D.h"
#include "../imgui/imgui.h"
#include "platform\opengl\OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: 
	Layer("Sandbox2D"),
	_CameraController(960.0f / 540.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	_SquareVA = SOMEENGINE::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.75f, 0.0f,
		 0.5f, -0.75f, 0.0f,
		 0.5f,  0.75f, 0.0f,
		-0.5f,  0.75f, 0.0f,
	};
	SOMEENGINE::Ref<SOMEENGINE::VertexBuffer> squareVB;
	squareVB = SOMEENGINE::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ SOMEENGINE::ShaderDataType::Float3, "a_Position" }
	});

	_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squreIndices[6] = {
		0, 1, 2,
		2, 3, 0,
	};
	SOMEENGINE::Ref<SOMEENGINE::IndexBuffer> squareIB;
	squareIB = SOMEENGINE::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t));

	_SquareVA->SetIndexBuffer(squareIB);

	_FlatColorShader = SOMEENGINE::Shader::Create("../GameEngine/assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(SOMEENGINE::Timestep ts)
{
	//Update
	_CameraController.OnUpdate(ts);

	//Render
	SOMEENGINE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	SOMEENGINE::RenderCommand::Clear();

	SOMEENGINE::Renderer::BeginScene(_CameraController.GetCamera());

	std::dynamic_pointer_cast<SOMEENGINE::OpenGLShader>(_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<SOMEENGINE::OpenGLShader>(_FlatColorShader)->UpdateUniformFloat4("u_Color", _SquareColor);

	SOMEENGINE::Renderer::Submit(_FlatColorShader, _SquareVA,glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	SOMEENGINE::Renderer::EndScene();
}

void Sandbox2D::OnEvent(SOMEENGINE::Event & event)
{
	_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color ", &_SquareColor.x);
	ImGui::End();
}
