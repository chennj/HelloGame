#include "sopch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "platform\opengl\OpenGLVertexArray.h"

namespace SOMEENGINE
{
	VertexArray * VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		SE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}