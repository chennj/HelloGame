// opengl Êý¾Ý¿â
// http://glm.g-truc.net/
// https://github.com/g-truc/glm
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