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
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		SE_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage;

		s_Data->QaudVA = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0, 0.0,
			 0.5f, -0.5f, 0.0f, 1.0, 0.0,
			 0.5f,  0.5f, 0.0f, 1.0, 1.0,
			-0.5f,  0.5f, 0.0f, 0.0, 1.0,
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

		s_Data->WhiteTexture = Texture2D::Create(1,1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("../GameEngine/assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		SE_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera & camera)
	{
		SE_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		SE_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		SE_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", color);
		// Bind white texture here
		s_Data->WhiteTexture->Bind();

		glm::mat4 modelTranform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0));
		s_Data->TextureShader->SetMat4("u_Model", modelTranform);

		s_Data->QaudVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QaudVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const Ref<Texture2D>& texture)
	{
		SE_PROFILE_FUNCTION();

		// u_Color equal white
		s_Data->TextureShader->SetFloat4("u_Color", {0.41,0.47,0.79,1.0});

		texture->Bind();

		glm::mat4 modelTranform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0));
		s_Data->TextureShader->SetMat4("u_Model", modelTranform);

		s_Data->QaudVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QaudVA);

	}
}