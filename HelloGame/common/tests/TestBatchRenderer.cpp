#include "TestBatchRenderer.h"

#include "../imgui/imgui.h"

#include "glm\gtc\matrix_transform.hpp"

#include "..\stb_image\stb_image.h"

#include <array>
#include <iostream>

namespace tests
{
	static const size_t MaxQuadCount	= 1000;
	static const size_t MaxVertexCount	= MaxQuadCount * 4;	// 1个矩形 = 2个三角形 = 4个顶点（2个顶点重合）
	static const size_t MaxIndexCount	= MaxQuadCount * 6;
	static const size_t MaxTextures		= 8;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
	};

	struct RendererData
	{
		GLuint QuadVA = 0;
		GLuint QuadVB = 0;
		GLuint QuadIB = 0;

		GLuint WhiteTexture			= 0;
		uint32_t WhiteTextureSlot	= 0;

		uint32_t IndexCount = 0;

		Vertex* QuadBuffer		= nullptr;
		Vertex* QuadBufferPtr	= nullptr;

		std::array<uint32_t, MaxTextures> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		BatchRenderer2D::Stats RenderStats;
	};

	static RendererData s_Data;

	static GLuint LoadTexture(const std::string& path)
	{
		int w, h, bits;

		stbi_set_flip_vertically_on_load(1);

		auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, 4);
		GLuint textureID;
		GLCall(glGenTextures(1, &textureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

		if (pixels)
			stbi_image_free(pixels);

		return textureID;
	}

	// --------------------- BatchRenderer2D -------------------------------

	void BatchRenderer2D::Init()
	{
		std::cout << "BatchRenderer2D::Init()" << std::endl;

		s_Data.QuadBuffer = new Vertex[MaxVertexCount];

		GLCall(glGenVertexArrays(1, &s_Data.QuadVA));
		GLCall(glBindVertexArray(s_Data.QuadVA));

		GLCall(glGenBuffers(1, &s_Data.QuadVB));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
		GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

		// 对应BatchRenderer.shader的 layout(location = 0) in vec3 a_Position;
		// 绑定_VB到_VA的0位置，并置为有效
		// 设置0位置的属性
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));
		// 对应BatchRenderer.shader的 layout(location = 1) in vec4 a_Color;
		// 绑定_VB到_VA的1位置，并置为有效
		// 设置1位置的属性
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));
		// 对应BatchRenderer.shader的 layout(location = 2) in vec2	a_TexCoord;
		// 绑定_VB到_VA的2位置，并置为有效
		// 设置2位置的属性
		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords)));
		// 对应BatchRenderer.shader的 layout(location = 3) in float a_TexIndex;
		// 绑定_VB到_VA的3位置，并置为有效
		// 设置3位置的属性
		GLCall(glEnableVertexAttribArray(3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexIndex)));

		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;
		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		GLCall(glGenBuffers(1, &s_Data.QuadIB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

		// 1x1 white texture
		GLCall(glGenTextures(1, &s_Data.WhiteTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, s_Data.WhiteTexture));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		uint32_t color = 0xffffffff;
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

		std::cout << "TextureSlots[0] = WhiteTexture:" << s_Data.WhiteTexture << std::endl;

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		for (size_t i = 1; i < MaxTextures; i++)
		{
			s_Data.TextureSlots[i] = 0;
		}
	}

	void BatchRenderer2D::Shutdown()
	{
		GLCall(glDeleteVertexArrays(1, &s_Data.QuadVA));
		GLCall(glDeleteBuffers(1, &s_Data.QuadVB));
		GLCall(glDeleteBuffers(1, &s_Data.QuadIB));

		GLCall(glDeleteTextures(1, &s_Data.WhiteTexture));

		delete[] s_Data.QuadBuffer;
	}

	void BatchRenderer2D::BeginBatch()
	{
		s_Data.QuadBufferPtr = s_Data.QuadBuffer;
	}

	void BatchRenderer2D::EndBatch()
	{
		GLsizeiptr size = (uint8_t*)s_Data.QuadBufferPtr - (uint8_t*)s_Data.QuadBuffer;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadBuffer));
	}

	void BatchRenderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, s_Data.TextureSlots[i]));
		}

		GLCall(glBindVertexArray(s_Data.QuadVA));
		GLCall(glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr));
		s_Data.RenderStats.DrawCount++;

		s_Data.IndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void BatchRenderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_Data.IndexCount >= MaxIndexCount)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureIndex = 0.0f;

		s_Data.QuadBufferPtr->Position = { position.x, position.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 0.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 0.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 1.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 1.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.IndexCount += 6;
		s_Data.RenderStats.QuadCount++;
	}

	void BatchRenderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID)
	{
		if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > (MaxTextures-1))
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		//vs2015不完全支持c11的constexpr -- 编译期优化
		//constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		/**
		*
		*	1. 查找纹理卡槽中是否已经有这个纹理了
		*	2. 如果纹理卡槽中还没有这个纹理，加进去
		*	这个东西会在Flush()中用到，如果没有这两段代码，
		*	Flush()中就会找不到纹理，从而显示黑框
		*/
		// 1.
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == textureID) {
				textureIndex = (float)i;
				break;
			}
		}
		// 2.
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureID;
			s_Data.TextureSlotIndex++;
		}
		// -----------------------------

		s_Data.QuadBufferPtr->Position = { position.x, position.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 0.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 0.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 1.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 1.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.IndexCount += 6;
		s_Data.RenderStats.QuadCount++;
	}

	const BatchRenderer2D::Stats BatchRenderer2D::GetStats()
	{
		return s_Data.RenderStats;
	}

	void const BatchRenderer2D::ResetStats()
	{
		memset(&s_Data.RenderStats, 0, sizeof(Stats));
	}


	// --------------------- TestBatchRenderer -------------------------------

	TestBatchRenderer::TestBatchRenderer()
		:
		_Translation(200, 200, 0),
		_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		std::cout << "TestBatchRenderer::TestBatchRenderer()" << std::endl;

		_Shader = std::make_unique<Shader>("dlsl/BatchRenderer.shader");
		GLCall(glUseProgram(_Shader->GetRendererID()));
		GLCall(auto loc = glGetUniformLocation(_Shader->GetRendererID(), "u_Textures"));
		int samples[MaxTextures];
		for (int i = 0; i < MaxTextures; i++)
		{
			samples[i] = i;
		}
		GLCall(glUniform1iv(loc, MaxTextures, samples));

		BatchRenderer2D::Init();

		_Tex01 = LoadTexture("../res/texture/texture-01.png");
		_Tex02 = LoadTexture("../res/texture/texture-02.png");

	}

	TestBatchRenderer::~TestBatchRenderer()
	{
		std::cout << "TestBatchRenderer::~TestBatchRenderer()" << std::endl;

		BatchRenderer2D::Shutdown();
	}

	void TestBatchRenderer::OnUpdate(float deltaTime)
	{
		GLCall(glClearColor(0.25f, 0.25f, 0.25f, 1));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		glm::mat4 model = glm::translate(glm::mat4(1.0f), _Translation);
		glm::mat4 mvp = _Proj * _View * model;

		_Shader->Bind();
		_Shader->SetUniformMat4f("u_MVP", mvp);
	}

	void TestBatchRenderer::OnRender()
	{
		BatchRenderer2D::ResetStats();
		BatchRenderer2D::BeginBatch();

		// 画背景小方块
		// 因为上面的 model 模型矩阵向右向上移动了200，所以x，y都必须从-200开始
		// 才能铺满窗口
		for (float y = -200.0f; y < 340.0f; y += 30.0f)
		{
			for (float x = -200.0f; x < 760.0f; x += 30.0f)
			{
				glm::vec4 color = { (x+1000) / 2000.0f, 0.2f, (y+1000) / 2000.0f, 1.0f };
				BatchRenderer2D::DrawQuad({ x,y }, { 25.0f,25.0f }, color);
			}
		}

		// 画25个小纹理
		for (int y = 0; y < 250; y += 50)
		{
			for (int x = 0; x < 250; x += 50)
			{
				GLuint tex = (x / 50 + y / 50) % 2 ? _Tex01 : _Tex02;
				BatchRenderer2D::DrawQuad({ x,y }, { 50,50 }, tex);
			}
		}

		// 画一个大纹理
		BatchRenderer2D::DrawQuad(_QuadPosition, { 100,100 }, _Tex02);

		BatchRenderer2D::EndBatch();

		BatchRenderer2D::Flush();
	}

	void TestBatchRenderer::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad Position", &_QuadPosition.x, 10.0f);
	}
}
