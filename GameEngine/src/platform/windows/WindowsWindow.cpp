#include "sopch.h"
#include "stuff\Log.h"
#include "WindowsWindow.h"

namespace SOMEENGINE
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps & props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps & props)
	{
		_Data.Title		= props.Title;
		_Data.Width		= props.Width;
		_Data.Height	= props.Height;

		SE_CORE_INFO("Creating Window {0} {1} {2}", props.Title.c_str(), props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			SE_ASSERT(success, "Could not initialize GLFW");

			s_GLFWInitialized = true;
		}

		_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(_Window);
		glfwSetWindowUserPointer(_Window, &_Data);
		SetVSync(true);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return _Data.VSync;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(_Window);
	}
}

