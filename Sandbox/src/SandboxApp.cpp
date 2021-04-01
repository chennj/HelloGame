// 资料地址 //////////////////////////////////////////////
// opengl 数学库
// http://glm.g-truc.net/
// https://github.com/g-truc/glm
// opengl shader compilation
// http://www.khronos.org/opengl/wiki/Shader_Compilation
// -----------------------------------------------------
// 知识点 ///////////////////////////////////////////////
// VertexBuffer、IndexBuffer只有放入VertexArray中才会起作用
// 我们观察到的物体	= Projection * View * Model * VerticesPosition(Mask)
//					= VP(PV) * Model * VerticesPosition(Mask)
//					= CAMERA * Model * VerticesPosition(Mask)
//					= MVP * VerticesPosition(Mask)
// Projection(Matrix) * View(Matrix) = Camera : 投影（矩阵）变换 * 视图（矩阵）变换 = 观察点位置
// Model-Matrix = Object : 模型变换 = 物体位置
// ----------------------------------------------------
#include "sopch.h"

#include <Some.h>

#include "../imgui/imgui.h"

class ExampleLayer : public SOMEENGINE::Layer
{
public:
	ExampleLayer() :Layer("Example") 
	{
	}

public:
	void OnUpdate() override
	{
		if (SOMEENGINE::Input::IsKeyPressed(SE_KEY_TAB))
		{
			SE_TRACE("Tab Key is Pressed!");
		}
	}

	void OnEvent(SOMEENGINE::Event& event) override
	{
		//auto v = SOMEENGINE::Input::GetMosuePosition();
		//SE_CORE_TRACE("{0},{1}", v.first, v.second);

		if (event.GetEventType() == SOMEENGINE::EventType::KeyPressed)
		{
			SOMEENGINE::KeyPressedEvent& e = (SOMEENGINE::KeyPressedEvent&)event;
			SE_TRACE("{0},{1}", event, (char)e.GetKeyCode());
		}
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Test");
		float col[3] = { 0.6f,0.6f,0.6f };
		ImGui::ColorEdit3("color", col);
		ImGui::End();
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