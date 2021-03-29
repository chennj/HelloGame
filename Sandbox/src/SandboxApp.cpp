// opengl Êý¾Ý¿â
// http://glm.g-truc.net/
// https://github.com/g-truc/glm
#include "sopch.h"

#include <Some.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}


class ExampleLayer : public SOMEENGINE::Layer
{
public:
	ExampleLayer() :Layer("Example") 
	{
		auto a = camera(0.5f, { 0.5f, 0.5f });
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
			SE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public SOMEENGINE::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new SOMEENGINE::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

SOMEENGINE::Application* SOMEENGINE::CreateApplication()
{
	return new Sandbox();
}