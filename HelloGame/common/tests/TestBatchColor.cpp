#include "TestBatchColor.h"

#include "../imgui/imgui.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include <iostream>

namespace tests
{
	TestBatchColor::TestBatchColor()
		:
		_Translation(200, 200, 0),
		_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		std::cout << "TestBatchColor construction" << std::endl;

		float positions[] = {
			//第一个矩形（两个三角形）
			-50.0f, -50.0f, 0.0f,	/*pos 0*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/
			 50.0f, -50.0f, 0.0f,	/*pos 1*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 1 color*/
			 50.0f,  50.0f, 0.0f,	/*pos 2*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 2 color*/
			-50.0f,  50.0f, 0.0f,	/*pos 3*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 3 color*/
			//第二个矩形（两个三角形）
			 150.0f, -50.0f, 0.0f,	/*pos 4*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/
			 250.0f, -50.0f, 0.0f,	/*pos 5*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 5 color*/
			 250.0f,  50.0f, 0.0f,	/*pos 6*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 6 color*/
			 150.0f,  50.0f, 0.0f,	/*pos 7*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 7 color*/
		};

		GLCall(glGenVertexArrays(1, &_VA));
		GLCall(glBindVertexArray(_VA));

		GLCall(glGenBuffers(1, &_VB));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _VB));
		GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * 7 * sizeof(float), positions, GL_STATIC_DRAW));

		// 对应Color.shader的 layout(location = 0) in vec3 position
		// 绑定_VB到_VA的0位置，并置为有效
		// 设置0位置的属性
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0));
		// 对应Color.shader的 layout(location = 1) in vec4 u_Color
		// 绑定_VB到_VA的1位置，并置为有效
		// 设置1位置的属性
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)12));

		unsigned int indices[] = {
			0, 1, 2,			//第1个三角形的三个顶点的索引（index）
			2, 3, 0,			//第2个三角形的三个顶点的索引
			4, 5, 6,			//第3个三角形的三个顶点的索引
			6, 7, 4				//第4个三角形的三个顶点的索引
		};

		GLCall(glGenBuffers(1, &_IB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

		_Shader = std::make_unique<Shader>("Color.shader");
	}

	TestBatchColor::~TestBatchColor()
	{
		std::cout << "TestBatchColor disstruction" << std::endl;
	}

	void TestBatchColor::OnUpdate(float deltaTime)
	{
		GLCall(glUseProgram(_Shader->GetRendererID()));
	}

	void TestBatchColor::OnRender()
	{
		GLCall(glClearColor(0, 0, 0, 1));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		glm::mat4 model = glm::translate(glm::mat4(1.0f), _Translation);
		glm::mat4 mvp = _Proj * _View * model;
		_Shader->Bind();
		_Shader->SetUniformMat4f("u_MVP", mvp);
		GLCall(glBindVertexArray(_VA));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IB));
		GLCall(glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr));
	}

	void TestBatchColor::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation", &_Translation.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
