#include "sopch.h"

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"

#include "glm/gtc/matrix_transform.hpp"

namespace SOMEENGINE
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads		= 10000;
		const uint32_t MaxVertices	= MaxQuads * 4;
		const uint32_t MaxIndices	= MaxQuads * 6;

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr	 = nullptr;

		static const uint32_t MaxTextureSlots	= 8;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;	// 0 = white texture
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init(const std::string& filePath)
	{
		SE_PROFILE_FUNCTION();

		s_Data.QuadVA = VertexArray::Create();

		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float,  "a_TexIndex" },
			{ ShaderDataType::Float,  "a_TilingFactor" },
		});

		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVA->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1,1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int samplers[s_Data.MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data.TextureShader = Shader::Create(filePath);	//"../GameEngine/assets/shaders/Texture.glsl"
		s_Data.TextureShader->Bind();
		//s_Data.TextureShader->SetInt("u_Texture", 0);
		s_Data.TextureShader->SetIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer2D::Init(const std::string& vertexFilePath, const std::string& fragmentFilePath)
	{

	}

	void Renderer2D::Shutdown()
	{
		SE_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera & camera)
	{
		SE_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		SE_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i, true);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color, float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, color, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color, float rotation)
	{
		SE_PROFILE_FUNCTION();

		const float textureIndex = 0.0f;	// White Texture
		const float tilingFactor = 1.0f;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		//s_Data.TextureShader->SetFloat4("u_Color", color);
		//s_Data.TextureShader->SetFloat1("u_TilingFactor", 1.0f);
		//// Bind white texture here
		//s_Data.WhiteTexture->Bind();

		//glm::mat4 modelTranform = 
		//	glm::translate(glm::mat4(1.0), position) 
		//	* glm::rotate(glm::mat4(1.0f),rotation,glm::vec3(0.0,0.0,1.0f))
		//	* glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0));
		//s_Data.TextureShader->SetMat4("u_Model", modelTranform);

		//s_Data.QuadVA->Bind();
		//RenderCommand::DrawIndexed(s_Data.QuadVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture2D>& texture, float tilingFactor, float rotation, glm::vec4& tinColor)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, texture, tilingFactor, rotation, tinColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const Ref<Texture2D>& texture, float tilingFactor, float rotation, glm::vec4& tinColor)
	{
		SE_PROFILE_FUNCTION();

		const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		// Old Path
		//// u_Color equal white
		//s_Data.TextureShader->SetFloat4("u_Color", tinColor);
		//s_Data.TextureShader->SetFloat1("u_TilingFactor", tilingFactor);

		//texture->Bind();

		//glm::mat4 modelTranform = 
		//	glm::translate(glm::mat4(1.0), position) 
		//	* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0, 0.0, 1.0f))
		//	* glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0));
		//s_Data.TextureShader->SetMat4("u_Model", modelTranform);

		//s_Data.QuadVA->Bind();
		//RenderCommand::DrawIndexed(s_Data.QuadVA);

	}
	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const Ref<Texture2D>& texture, glm::vec4 & color)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, rotation, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const Ref<Texture2D>& texture, glm::vec4 & color)
	{
		SE_PROFILE_FUNCTION();

		// u_Color equal white
		s_Data.TextureShader->SetFloat4("u_Color", color);

		texture->Bind();

		glm::mat4 modelTranform =
			glm::translate(glm::mat4(1.0), position)
			* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0, 0.0, 1.0f))
			* glm::scale(glm::mat4(1.0), glm::vec3(size.x, size.y, 1.0));
		s_Data.TextureShader->SetMat4("u_Model", modelTranform);

		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);

	}
}