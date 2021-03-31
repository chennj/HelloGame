#include "sopch.h"

#include "OpenGLContext.h"

#include "glad\glad.h"
#include "GLFW\glfw3.h"

namespace SOMEENGINE
{
	OpenGLContext::OpenGLContext(GLFWwindow * windowHandle)
		: _WindowHandle(windowHandle)
	{
		SE_CORE_ASSERT(_WindowHandle, "Window handle is NULL!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SE_CORE_ASSERT(status, "Could not initialize GLAD");

		SE_CORE_INFO("OpenGL Info");
		SE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		SE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		SE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		
		glfwSwapBuffers(_WindowHandle);
	}
}