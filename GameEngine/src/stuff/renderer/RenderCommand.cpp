#include "sopch.h"
#include "RenderCommand.h"
#include "platform\opengl\OpenGLRendererAPI.h"

namespace SOMEENGINE
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}