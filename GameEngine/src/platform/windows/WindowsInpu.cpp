#include "sopch.h"
#include "stuff\core\Input.h"

#include "stuff\core\Application.h"
#include "GLFW\glfw3.h"

namespace SOMEENGINE
{
	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool  Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return{ (float)xpos,(float)ypos };
	}

	float Input::GetMouseX()
	{
		auto a = Input::GetMousePosition();
		return std::get<0>(a);
	}

	float Input::GetMouseY()
	{
		auto a = GetMousePosition();
		return a.second;
	}
}