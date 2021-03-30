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

	}

	void OpenGLContext::SwapBuffers()
	{
		
		glfwSwapBuffers(_WindowHandle);
	}
}