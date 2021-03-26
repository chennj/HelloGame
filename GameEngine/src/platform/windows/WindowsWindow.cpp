#include "sopch.h"
#include "stuff\Log.h"
#include "WindowsWindow.h"

#include "stuff\events\ApplicationEvent.h"
#include "stuff\events\KeyEvent.h"
#include "stuff\events\MouseEvent.h"

// glad.h必须包含在glfw的前面
#include "glad\glad.h"
#include "GLFW\glfw3.h"

namespace SOMEENGINE
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error_code, const char* description)
	{
		SE_CORE_ERROR("GLFW ERROR ({0}): {1}", error_code, description);
	}

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
			glfwSetErrorCallback(GLFWErrorCallback);
			SE_CORE_ASSERT(success, "Could not initialize GLFW");
			s_GLFWInitialized = true;
		}

		_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(_Window);
		// GLAD---
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SE_CORE_ASSERT(status, "Could not initialize GLAD");
		// -------
		glfwSetWindowUserPointer(_Window, &_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);

		});

		glfwSetWindowCloseCallback(_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xpos, (float)ypos);
			data.EventCallback(event);
		});
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

