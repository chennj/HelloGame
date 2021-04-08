#include "sopch.h"

#include "Buffer.h"
#include "Renderer.h"
#include "platform\opengl\OpenGLBuffer.h"

namespace SOMEENGINE
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		SE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		SE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}