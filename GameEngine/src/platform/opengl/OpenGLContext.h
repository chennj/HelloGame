#pragma once

#include "stuff\renderer\GraphicsContext.h"

struct GLFWwindow;

namespace SOMEENGINE
{
	class OpenGLContext : public GraphicsContext
	{
	private:
		GLFWwindow* _WindowHandle;

	public:
		OpenGLContext(GLFWwindow* windowHandle);

	public:
		virtual void Init()override;
		virtual void SwapBuffers()override;
	};
}