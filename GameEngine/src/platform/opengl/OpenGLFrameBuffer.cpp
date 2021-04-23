#include "sopch.h"

#include "OpenGLBuffer.h"
#include "OpenGLFrameBuffer.h"

#include "glad\glad.h"
#include "OpenGLDebug.h"

namespace SOMEENGINE
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification & spec)
		:_Specification(spec)
	{
		Invalide();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &_RendererID);
	}

	void OpenGLFrameBuffer::Invalide()
	{
		if (OPENGL_VERSION >= 4.5)
		{
			glCreateFramebuffers(1, &_RendererID);
			glBindFramebuffer(GL_FRAMEBUFFER, _RendererID);

			glCreateTextures(GL_TEXTURE_2D, 1, &_ColorAttachment);
			glBindTexture(GL_TEXTURE_2D, _ColorAttachment);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Specification.Width, _Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _ColorAttachment, 0);

			glCreateTextures(GL_TEXTURE_2D,1, &_DepthAttachment);
			glBindTexture(1,_DepthAttachment);
			//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _Specification.Width, _Specification.Height);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _Specification.Width, _Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0);

		}
		else
		{
			GLCall(glGenFramebuffers(1, &_RendererID));
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _RendererID));

			GLCall(glGenTextures(1, &_ColorAttachment));
			GLCall(glBindTexture(GL_TEXTURE_2D, _ColorAttachment));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, _Specification.Width, _Specification.Height));
			//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Specification.Width, _Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _ColorAttachment, 0));
			//GLCall(glBindTexture(GL_TEXTURE_2D, 0));

			GLCall(glGenTextures(1, &_DepthAttachment));
			GLCall(glBindTexture(GL_TEXTURE_2D, _DepthAttachment));
			GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _Specification.Width, _Specification.Height));
			//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _Specification.Width, _Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));
			
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0));
			//GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}

		SE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _RendererID);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}