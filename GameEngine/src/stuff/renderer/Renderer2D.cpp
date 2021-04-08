#include "sopch.h"

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"

#include "platform\opengl\OpenGLShader.h"

namespace SOMEENGINE
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> _QaudVA;
		Ref<Shader> _FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage;

		s_Data->_QaudVA = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.75f, 0.0f,
			 0.5f, -0.75f, 0.0f,
			 0.5f,  0.75f, 0.0f,
			-0.5f,  0.75f, 0.0f,
		};
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});

		s_Data->_QaudVA->AddVertexBuffer(squareVB);

		uint32_t squreIndices[6] = {
			0, 1, 2,
			2, 3, 0,
		};
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t));

		s_Data->_QaudVA->SetIndexBuffer(squareIB);

		s_Data->_FlatColorShader = Shader::Create("../GameEngine/assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera & camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->_FlatColorShader)->UpdateUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->_FlatColorShader)->UpdateUniformMat4("u_Model", glm::mat4(1.0));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->_FlatColorShader)->UpdateUniformFloat4("u_Color", color);

		s_Data->_QaudVA->Bind();
		RenderCommand::DrawIndexed(s_Data->_QaudVA);
	}
}