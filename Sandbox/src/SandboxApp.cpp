#include "sopch.h"

#include <Some.h>

class ExampleLayer : public SOMEENGINE::Layer
{
public:
	ExampleLayer() :Layer("Example") {}

public:
	void OnUpdate() override
	{
		SE_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(SOMEENGINE::Event& event) override
	{
		SE_TRACE("{0}", event);
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