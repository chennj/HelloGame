#include "sopch.h"

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"

#include "glm/gtc/matrix_transform.hpp"

namespace SOMEENGINE
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QaudVA;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage;

		s_Data->QaudVA = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.75f, 0.0f, 0.0, 0.0,
			 0.5f, -0.75f, 0.0f, 1.0, 0.0,
			 0.5f,  0.75f, 0.0f, 1.0, 1.0,
			-0.5f,  0.75f, 0.0f, 0.0, 1.0,
		};
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});

		s_Data->QaudVA->AddVertexBuffer(squareVB);

		uint32_t squreIndices[6] = {
			0, 1, 2,
			2, 3, 0,
		};
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t));

		s_Data->QaudVA->SetIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::Create("../GameEngine/assets/shaders/FlatColor.glsl");

		s_Data->TextureShader = Shader::Create("../GameEngine/assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera & camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 modelTranform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0));
		s_Data->FlatColorShader->SetMat4("u_Model", modelTranform);

		s_Data->QaudVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QaudVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();

		glm::mat4 modelTranform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0));
		s_Data->TextureShader->SetMat4("u_Model", modelTranform);

		texture->Bind();

		s_Data->QaudVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QaudVA);

	}
}