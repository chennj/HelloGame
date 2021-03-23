#include "TestBatchTexture2D.h"

#include "../imgui/imgui.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "..\stb_image\stb_image.h"

namespace tests
{

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

	TestBatchTexture2D::TestBatchTexture2D()
		:_Translation(200, 200, 0),
		_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float positions[] = {
			//第一个矩形（两个三角形）
			-50.0f, -50.0f,  0.0f,	/*pos 0*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	0.0f, 0.0f,	/*pos 0 texture*/	0.0f,	/*texture index*/
			 100.0f,-50.0f,  0.0f,	/*pos 1*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	1.0f, 0.0f,	/*pos 1 texture*/	0.0f,	/*texture index*/
			 100.0f, 100.0f, 0.0f,	/*pos 2*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	1.0f, 1.0f,	/*pos 2 texture*/	0.0f,	/*texture index*/
			-50.0f,  100.0f, 0.0f,	/*pos 3*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	0.0f, 1.0f,	/*pos 3 texture*/	0.0f,	/*texture index*/
			//第二个矩形（两个三角形）
			 250.0f, -50.0f, 0.0f,	/*pos 4*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	0.0f, 0.0f,	/*pos 4 texture*/	1.0f,	/*texture index*/
			 400.0f, -50.0f, 0.0f,	/*pos 5*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	1.0f, 0.0f,	/*pos 5 texture*/	1.0f,	/*texture index*/
			 400.0f,  100.0f,0.0f,	/*pos 6*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	1.0f, 1.0f,	/*pos 6 texture*/	1.0f,	/*texture index*/
			 250.0f,  100.0f,0.0f,	/*pos 7*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	0.0f, 1.0f,	/*pos 7 texture*/	1.0f,	/*texture index*/
		};

		GLCall(glGenVertexArrays(1, &_VA));
		GLCall(glBindVertexArray(_VA));

		GLCall(glGenBuffers(1, &_VB));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _VB));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

		// 对应ColorAndTexture.shader的 layout(location = 0) in vec3 a_Position;
		// 绑定_VB到_VA的0位置，并置为有效
		// 设置0位置的属性
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), 0));	
		// 对应ColorAndTexture.shader的 layout(location = 1) in vec4 a_Color;
		// 绑定_VB到_VA的1位置，并置为有效
		// 设置1位置的属性
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)12));
		// 对应ColorAndTexture.shader的 layout(location = 1) in vec2	a_TexCoord;
		// 绑定_VB到_VA的2位置，并置为有效
		// 设置2位置的属性
		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)28));
		// 对应ColorAndTexture.shader的 layout(location = 1) in float a_TexIndex;
		// 绑定_VB到_VA的2位置，并置为有效
		// 设置3位置的属性
		GLCall(glEnableVertexAttribArray(3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)36));

		unsigned int indices[] = {
			0, 1, 2,			//第1个矩形的第1个三角形的三个顶点的索引（在positions[]的位置）
			2, 3, 0,			//第1个矩形的第2个三角形的三个顶点的索引
			4, 5, 6,			//第2个矩形的第1个三角形的三个顶点的索引
			6, 7, 4				//第2个矩形的第2个三角形的三个顶点的索引
		};

		GLCall(glGenBuffers(1, &_IB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

		_Tex01 = LoadTexture("../res/texture/texture-01.png");
		_Tex02 = LoadTexture("../res/texture/texture-02.png");

		_Shader = std::make_unique<Shader>("ColorAndTexture.shader");
		GLCall(glUseProgram(_Shader->GetRendererID()));
		GLCall(auto loc = glGetUniformLocation(_Shader->GetRendererID(), "u_Textures"));
		int samples[2] = { 0,1 };
		GLCall(glUniform1iv(loc, 2, samples));

		GLCall(glActiveTexture(GL_TEXTURE0 + 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, _Tex01));
		GLCall(glActiveTexture(GL_TEXTURE0 + 1));
		GLCall(glBindTexture(GL_TEXTURE_2D, _Tex02));
		//opengl version 330 不支持
		//glBindTextureUnit(0, _Tex01);
		//glBindTextureUnit(1, _Tex02);
	}

	TestBatchTexture2D::~TestBatchTexture2D()
	{
		GLCall(glDeleteTextures(1, &_Tex01));
		GLCall(glDeleteTextures(1, &_Tex02));
	}

	void TestBatchTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestBatchTexture2D::OnRender()
	{
		GLCall(glClearColor(0.25f, 0.25f, 0.25f, 1));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		_Shader->Bind();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), _Translation);
		glm::mat4 mvp = _Proj * _View * model;
		_Shader->SetUniformMat4f("u_MVP", mvp);

		GLCall(glBindVertexArray(_VA));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IB));
		GLCall(glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr));
	}

	void TestBatchTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation", &_Translation.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
