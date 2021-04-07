#include "sopch.h"
#include "WindowsInput.h"

#include "stuff\core\Application.h"
#include "GLFW\glfw3.h"

namespace SOMEENGINE
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool  WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMosuePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return{ (float)xpos,(float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto a = GetMosuePositionImpl();
		return std::get<0>(a);
	}

	float WindowsInput::GetMosueYImpl()
	{
		auto a = GetMosuePositionImpl();
		return a.second;
	}
}