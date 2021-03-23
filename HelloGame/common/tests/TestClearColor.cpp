#include "TestClearColor.h"

#include "../Renderer.h"
#include "../imgui/imgui.h"

namespace tests
{
	TestClearColor::TestClearColor()
		:_ClearColor{ 0.2f,0.3f,0.8f,1.0f }
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(float deltaTime)
	{
	}

	void TestClearColor::OnRender()
	{
		GLCall(glClearColor(_ClearColor[0], _ClearColor[1], _ClearColor[2], _ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", _ClearColor);
	}

}
