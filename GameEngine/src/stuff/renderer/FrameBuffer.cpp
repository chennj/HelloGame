#include "sopch.h"

#include "FrameBuffer.h"
#include "Renderer.h"
#include "platform\opengl\OpenGLFrameBuffer.h"

namespace SOMEENGINE
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification & spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFrameBuffer>(spec);
		}

		SE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}