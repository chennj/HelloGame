#include "sopch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "platform\opengl\OpenGLVertexArray.h"

namespace SOMEENGINE
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}

		SE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}